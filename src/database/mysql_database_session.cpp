#include "mysql_database_session.h"
#include "common/logger.h"

#include <boost/asio/use_awaitable.hpp>

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
