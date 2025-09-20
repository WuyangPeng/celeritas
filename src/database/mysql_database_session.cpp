#include "mysql_database_session.h"

celeritas::mysql_database_session::mysql_database_session(boost::asio::io_context& io_context, boost::asio::ssl::context* ssl_context)
    : connection_{ get_any_connection(io_context, ssl_context) }
{
}

celeritas::mysql_database_session::connection_type celeritas::mysql_database_session::get_any_connection(boost::asio::io_context& io_context, boost::asio::ssl::context* ssl_context)
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

celeritas::mysql_database_session::awaitable_type celeritas::mysql_database_session::async_connect(const std::string_view& host,
                                                                                                   uint16_t port,
                                                                                                   const std::string_view& user,
                                                                                                   const std::string_view& password,
                                                                                                   const std::string_view& db_name)
{
    boost::mysql::connect_params connect_params{};
    connect_params.server_address.emplace_host_and_port(host.data(), port);
    connect_params.username = user;
    connect_params.password = password;
    connect_params.database = db_name;

    co_await connection_.async_connect(connect_params, boost::asio::use_awaitable);
}

celeritas::mysql_database_session::results_type celeritas::mysql_database_session::async_query(const std::string_view& sql)
{
    boost::mysql::results results{};
    co_await connection_.async_execute(sql, results, boost::asio::use_awaitable);

    co_return results;
}
