#include "basis_database.tpp"
#include "basis_database_manager.h"
#include "database_change_type.h"
#include "database_data_type.h"
#include "database_field.h"
#include "mysql_database_session.h"
#include "common/celeritas_error.h"
#include "common/logger.h"
#include "common/noexcept_safe_call_and_log.h"
#include "detail/mysql_statement_generator.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/asio/use_awaitable.hpp>

#include <ranges>

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
    : connection_{ get_any_connection(io_context, ssl_context) },
      mysql_parameter_{ host, port, user, password, db_name }
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
    co_await connection_.async_connect(mysql_parameter_.get_connect_params_type(), boost::asio::use_awaitable);

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

    co_return results_type{};
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
            co_await async_query(mysql_statement_generator::generate_update_statement(database));
            co_return;
        }
        case database_change_type::insert_type:
        {
            co_await async_query(mysql_statement_generator::generate_insert_statement(database));
            co_return;
        }
        case database_change_type::delete_type:
        {
            co_await async_query(mysql_statement_generator::generate_delete_statement(database));
            co_return;
        }
    }

    co_return;
}

celeritas::database_session::basis_database_manager_awaitable_type celeritas::mysql_database_session::select_one(const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container)
{
    const auto result = co_await async_query(mysql_statement_generator::generate_select_statement(field_name_container, *database) + " LIMIT 1;");

    auto select = database->get_select();
    if (const auto& rows = result.rows();
        !rows.empty())
    {
        auto index = 0;
        for (const auto& element : rows.at(0))
        {
            select.modify(get_basis_database(field_name_container.at(index), element));

            ++index;
        }
    }

    co_return select;
}

celeritas::database_session::result_container_awaitable_type celeritas::mysql_database_session::select_all(const basis_database_manager_const_shared_ptr& database, const database_field_container& field_name_container)
{
    const auto result = co_await async_query(mysql_statement_generator::generate_select_statement(field_name_container, *database) + ";");

    result_container container{};

    for (const auto& entity : result.rows())
    {
        auto select = database->get_select();
        auto index = 0;
        for (const auto& value : entity)
        {
            select.modify(get_basis_database(field_name_container.at(index), value));
            ++index;
        }

        container.emplace_back(select);
    }

    co_return container;
}

celeritas::mysql_database_session::connection_type celeritas::mysql_database_session::get_any_connection(io_context_type& io_context, ssl_io_context_type* ssl_context)
{
    if (ssl_context == nullptr)
    {
        return { io_context };
    }

    boost::mysql::any_connection_params any_connection_params{};
    any_connection_params.ssl_context = ssl_context;

    return { io_context, any_connection_params };
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

celeritas::basis_database celeritas::mysql_database_session::get_basis_database(const database_field& field_name, const field_view_type& row_view)
{
    switch (field_name.get_data_type())
    {
        case database_data_type::string_type:
        {
            return basis_database{ field_name.get_field_name(), row_view.as_string() };
        }

        case database_data_type::int32_type:
        case database_data_type::int32_count_type:
        {
            return basis_database{ field_name.get_field_name(), boost::numeric_cast<int32_t>(row_view.as_int64()) };
        }

        case database_data_type::int64_type:
        case database_data_type::int64_count_type:
        {
            return basis_database{ field_name.get_field_name(), row_view.as_int64() };
        }
        case database_data_type::double_type:
        {
            return basis_database{ field_name.get_field_name(), row_view.as_double() };
        }

        case database_data_type::bool_type:
        {
            return basis_database{ field_name.get_field_name(), row_view.as_int64() != 0 };
        }

        case database_data_type::string_array_type:
        {
            basis_database::string_array result{};
            split(result, row_view.as_string(), boost::is_any_of("|"), boost::token_compress_off);

            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::int32_array_type:
        {
            const std::string value{ row_view.as_string() };
            auto split_view = value | std::views::split('|');

            auto int_view = split_view | std::views::transform([](const auto& subrange) {
                const std::string result{ subrange.begin(), subrange.end() };
                return boost::lexical_cast<int32_t>(result);
            });
            const basis_database::int32_array result{ int_view.begin(), int_view.end() };

            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::int64_array_type:
        {
            const std::string value{ row_view.as_string() };
            auto split_view = value | std::views::split('|');

            auto int_view = split_view | std::views::transform([](const auto& subrange) {
                const std::string result{ subrange.begin(), subrange.end() };
                return boost::lexical_cast<int64_t>(result);
            });
            const basis_database::int64_array result{ int_view.begin(), int_view.end() };

            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::double_array_type:
        {
            const std::string value{ row_view.as_string() };
            auto split_view = value | std::views::split('|');

            auto int_view = split_view | std::views::transform([](const auto& subrange) {
                const std::string result{ subrange.begin(), subrange.end() };
                return boost::lexical_cast<double>(result);
            });
            const basis_database::double_array result{ int_view.begin(), int_view.end() };

            return basis_database{ field_name.get_field_name(), result };
        }

        default:
        {
            return basis_database{ field_name.get_field_name(), ""s };
        }
    }
}
