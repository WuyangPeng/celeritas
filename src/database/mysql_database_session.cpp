#include "basis_database_manager.h"
#include "database_change_type.h"
#include "mysql_database_session.h"
#include "common/celeritas_error.h"
#include "common/logger.h"

#include <boost/asio/use_awaitable.hpp>

using namespace std::literals;

celeritas::mysql_database_session::mysql_database_session(const std::string_view& host,
                                                          const int port,
                                                          const std::string_view& user,
                                                          const std::string_view& password,
                                                          const std::string_view& uri,
                                                          const std::string_view& db_name,
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
    try
    {
        connection_.close();
    }
    catch (const boost::system::system_error& error)
    {
        LOG_CHANNEL(database_channel, warning) << "Error closing MySQL connection in destructor: " << error.what();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(database_channel, error) << "Unexpected error during MySQL connection close: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(database_channel, fatal) << "Unexpected error during MySQL connection close unknown exception";
    }
}

celeritas::mysql_database_session::void_awaitable_type celeritas::mysql_database_session::async_connect()
{
    boost::mysql::connect_params connect_params{};
    connect_params.server_address.emplace_host_and_port(host_, port_);
    connect_params.username = user_;
    connect_params.password = password_;
    connect_params.database = db_name_;

    co_await connection_.async_connect(connect_params, boost::asio::use_awaitable);
}

celeritas::mysql_database_session::results_awaitable_type celeritas::mysql_database_session::async_query(const std::string_view& sql)
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

celeritas::mysql_database_session::void_awaitable_type celeritas::mysql_database_session::save(const basis_database_manager_shared_ptr& database)
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

celeritas::mysql_database_session::results_awaitable_type celeritas::mysql_database_session::async_execute_query(const std::string_view& sql)
{
    results_type results{};

    co_await connection_.async_execute(sql, results, boost::asio::use_awaitable);

    co_return results;
}

celeritas::mysql_database_session::results_awaitable_type celeritas::mysql_database_session::async_handle_and_retry(const std::string_view& sql, const error_code_type& error_code)
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

std::string celeritas::mysql_database_session::generate_insert_statement(const basis_database_manager_shared_ptr& database)
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

std::string celeritas::mysql_database_session::generate_update_statement(const basis_database_manager_shared_ptr& database)
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

std::string celeritas::mysql_database_session::generate_delete_statement(const basis_database_manager_shared_ptr& database)
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
