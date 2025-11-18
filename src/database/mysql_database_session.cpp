#include "basis_database.tpp"
#include "basis_database_manager.h"
#include "database_change_type.h"
#include "database_data_type.h"
#include "database_field.h"
#include "mysql_database_session.h"
#include "common/celeritas_error.h"
#include "common/logger.h"
#include "common/noexcept_safe_call_and_log.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/asio/use_awaitable.hpp>

using namespace std::literals;

celeritas::mysql_database_session::mysql_database_session(const std::string_view host,
                                                          const int port,
                                                          const std::string_view user,
                                                          const std::string_view password,
                                                          const std::string_view uri,
                                                          const std::string_view db_name,
                                                          int expire_seconds,
                                                          io_context_type& io_context,
                                                          ssl_io_context_type* ssl_context)
    : host_{ host },
      port_{ port },
      user_{ user },
      password_{ password },
      db_name_{ db_name },
      connection_{ get_any_connection(io_context, ssl_context) }
{
}

celeritas::mysql_database_session::~mysql_database_session() noexcept
{
    noexcept_safe_call_and_log([this] {
                                   connection_.close();
                               },
                               common_channel,
                               "Unexpected error during MySQL connection close: ");
}

celeritas::mysql_database_session::void_awaitable_type celeritas::mysql_database_session::async_connect()
{
    boost::mysql::connect_params connect_params{};
    connect_params.server_address.emplace_host_and_port(host_, port_);
    connect_params.username = user_;
    connect_params.password = password_;
    connect_params.database = db_name_;

    co_await connection_.async_connect(connect_params, boost::asio::use_awaitable);

    co_return;
}

celeritas::mysql_database_session::results_awaitable_type celeritas::mysql_database_session::async_query(const std::string& sql)
{
    std::optional<boost::system::error_code> retry_error{};

    try
    {
        co_return co_await async_execute_query(sql);
    }
    catch (const boost::system::system_error& error)
    {
        LOG_CHANNEL(database_channel, error) << "async_query exception" << error.what();

        retry_error = error.code();
    }
    catch (...)
    {
        LOG_CHANNEL(database_channel, fatal) << "async_query unknown exception";
        throw;
    }

    if (retry_error.has_value())
    {
        co_return co_await async_handle_and_retry(sql, retry_error.value());
    }
}

celeritas::database_session::bool_awaitable_type celeritas::mysql_database_session::is_health()
{
    try
    {
        co_await connection_.async_ping(boost::asio::use_awaitable);

        co_return true;
    }
    catch (const boost::system::system_error& error)
    {
        LOG_CHANNEL(database_channel, warning) << "MySQL health check failed with error: " << error.what();
        co_return false;
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(database_channel, error) << "MySQL health check failed with unexpected exception: " << error.what();
        co_return false;
    }
    catch (...)
    {
        LOG_CHANNEL(database_channel, fatal) << "MySQL health check failed with unknown exception";
        co_return false;
    }
}

celeritas::mysql_database_session::void_awaitable_type celeritas::mysql_database_session::execute_changes(const basis_database_manager_const_shared_ptr& database)
{
    switch (database->get_change_type())
    {
        case database_change_type::select_type:
        {
            throw celeritas_error("change type is select.");
        }

        case database_change_type::update_type:
        {
            co_await async_query(generate_update_statement(database));
            co_return;
        }
        case database_change_type::insert_type:
        {
            co_await async_query(generate_insert_statement(database));
            co_return;
        }
        case database_change_type::delete_type:
        {
            co_await async_query(generate_delete_statement(database));
            co_return;
        }
    }

    co_return;
}

celeritas::database_session::basis_database_manager_awaitable_type celeritas::mysql_database_session::select_one(const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container)
{
    auto result = co_await async_query(generate_select_statement(field_name_container, *database) + " LIMIT 1;");

    basis_database_manager select{ database->get_database_type(), database->get_database_name(), database_change_type::select_type, database->get_key() };

    if (const auto& rows = result.rows();
        !rows.empty())
    {
        auto index = 0;
        for (const auto& value : rows.at(0))
        {
            select.modify(get_basis_database(field_name_container.at(index), value));

            ++index;
        }
    }

    co_return select;
}

celeritas::database_session::result_container_awaitable_type celeritas::mysql_database_session::select_all(const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container)
{
    auto result = co_await async_query(generate_select_statement(field_name_container, *database) + ";");

    result_container result_container{};

    for (const auto& entity : result.rows())
    {
        basis_database_manager select{ database->get_database_type(), database->get_database_name(), database_change_type::select_type, database->get_key() };

        auto index = 0;
        for (const auto& value : entity)
        {
            select.modify(get_basis_database(field_name_container.at(index), value));
            ++index;
        }

        result_container.emplace_back(select);
    }

    co_return result_container;
}

celeritas::mysql_database_session::connection_type celeritas::mysql_database_session::get_any_connection(io_context_type& io_context, ssl_io_context_type* ssl_context)
{
    if (ssl_context == nullptr)
    {
        return { io_context };
    }
    else
    {
        boost::mysql::any_connection_params any_connection_params{};
        any_connection_params.ssl_context = ssl_context;

        return { io_context, any_connection_params };
    }
}

celeritas::mysql_database_session::results_awaitable_type celeritas::mysql_database_session::async_execute_query(const std::string& sql)
{
    results_type results{};

    co_await connection_.async_execute(sql, results, boost::asio::use_awaitable);

    co_return results;
}

celeritas::mysql_database_session::results_awaitable_type celeritas::mysql_database_session::async_handle_and_retry(const std::string& sql, const error_code_type& error_code)
{
    if (error_code == boost::asio::error::eof ||
        error_code == boost::asio::error::broken_pipe ||
        error_code == boost::asio::error::connection_reset)
    {
        LOG_CHANNEL(database_channel, warning) << "Database connection lost. Trying to reconnect...";

        try
        {
            co_await async_connect();

            LOG_CHANNEL(database_channel, info) << "Database reconnected successfully. Retrying query.";

            co_return co_await async_execute_query(sql);
        }
        catch (const std::exception& reconnect_error)
        {
            LOG_CHANNEL(database_channel, error) << "Reconnection failed: " << reconnect_error.what();

            throw;
        }
    }

    throw;
}

std::string celeritas::mysql_database_session::generate_insert_statement(const basis_database_manager_const_shared_ptr& database)
{
    std::string result{};

    result += "INSERT INTO `"s + database->get_database_name().data() + "`(";

    const auto container = database->get_database();
    auto index = 1;
    for (const auto& value : container)
    {
        result += "`";
        result += value.get_field_name();
        result += "`";

        if (index != container.get_size())
        {
            result += " , ";
        }

        ++index;
    }

    result += ") VALUES(";

    index = 1;
    for (const auto& value : container)
    {
        result += value.get_quotation_mark_string();

        if (index != container.get_size())
        {
            result += " , ";
        }

        ++index;
    }

    result += ");";

    return result;
}

std::string celeritas::mysql_database_session::generate_update_statement(const basis_database_manager_const_shared_ptr& database)
{
    std::string result{};

    result += "UPDATE `"s + database->get_database_name().data() + "` SET ";

    const auto container = database->get_database();
    auto index = 1;
    for (const auto& value : container)
    {
        result += "`";
        result += value.get_field_name();
        result += "` = ";
        result += value.get_sql_field_string();

        if (index != container.get_size())
        {
            result += " , ";
        }

        ++index;
    }

    result += "WHERE ";

    const auto key = database->get_key();

    index = 1;
    for (const auto& value : key)
    {
        result += "`";
        result += value.get_field_name();
        result += "` = ";
        result += value.get_sql_field_string();

        if (index != key.get_size())
        {
            result += " AND ";
        }

        ++index;
    }

    result += " LIMIT 1;";

    return result;
}

std::string celeritas::mysql_database_session::generate_delete_statement(const basis_database_manager_const_shared_ptr& database)
{
    std::string result{};

    result += "DELETE FROM `"s + database->get_database_name().data() + "` WHERE ";

    const auto key = database->get_key();
    auto index = 1;
    for (const auto& value : key)
    {
        result += "`";
        result += value.get_field_name();
        result += "` = ";
        result += value.get_sql_field_string();

        if (index != key.get_size())
        {
            result += " AND ";
        }

        ++index;
    }

    result += " LIMIT 1;";

    return result;
}

std::string celeritas::mysql_database_session::generate_select_statement(const database_field_container& field_name_container, const basis_database_manager& database)
{
    std::string result{};

    result += "SELECT ";

    auto index = 1u;
    for (const auto& value : field_name_container)
    {
        result += "`";
        result += value.get_field_name();
        result += "`";

        if (index != field_name_container.size())
        {
            result += " , ";
        }

        ++index;
    }

    result += "FROM `"s + database.get_database_name().data() + "` WHERE ";

    auto keyIndex = 1;
    for (const auto key = database.get_key();
         const auto& value : key)
    {
        result += "`";
        result += value.get_field_name();
        result += "` = ";

        result += value.get_sql_field_string();

        if (keyIndex != key.get_size())
        {
            result += " AND ";
        }

        ++keyIndex;
    }

    return result;
}

celeritas::basis_database celeritas::mysql_database_session::get_basis_database(const database_field& field_name, const boost::mysql::field_view& row_view)
{
    switch (field_name.get_data_type())
    {
        case database_data_type::string_type:
            return basis_database{ field_name.get_field_name(), row_view.as_string() };

        case database_data_type::int32_type:
        case database_data_type::int32_count_type:
            return basis_database{ field_name.get_field_name(), boost::numeric_cast<int32_t>(row_view.as_int64()) };

        case database_data_type::int64_type:
        case database_data_type::int64_count_type:
            return basis_database{ field_name.get_field_name(), row_view.as_int64() };

        case database_data_type::double_type:
            return basis_database{ field_name.get_field_name(), row_view.as_double() };

        case database_data_type::bool_type:
            return basis_database{ field_name.get_field_name(), row_view.as_int64() != 0 };

        case database_data_type::string_array_type:
        {
            const std::string column{ row_view.as_string() };
            basis_database::string_array element{};
            split(element, column, boost::is_any_of("|"), boost::token_compress_off);

            return basis_database{ field_name.get_field_name(), element };
        }

        case database_data_type::int32_array_type:
        {
            const std::string column{ row_view.as_string() };
            basis_database::string_array element{};
            split(element, column, boost::is_any_of("|"), boost::token_compress_off);

            basis_database::int32_array result{};
            for (const auto& value : element)
            {
                result.emplace_back(boost::lexical_cast<int32_t>(value));
            }
            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::int64_array_type:
        {
            const std::string column{ row_view.as_string() };
            basis_database::string_array element{};
            split(element, column, boost::is_any_of("|"), boost::token_compress_off);

            basis_database::int64_array result{};
            for (const auto& value : element)
            {
                result.emplace_back(boost::lexical_cast<int64_t>(value));
            }
            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::double_array_type:
        {
            const std::string column{ row_view.as_string() };
            basis_database::string_array element{};
            split(element, column, boost::is_any_of("|"), boost::token_compress_off);

            basis_database::double_array result{};
            for (const auto& value : element)
            {
                result.emplace_back(boost::lexical_cast<double>(value));
            }
            return basis_database{ field_name.get_field_name(), element };
        }

        default:
            return basis_database{ field_name.get_field_name(), ""s };
    }
}
