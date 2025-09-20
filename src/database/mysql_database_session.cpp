#include "mysql_database_session.h"

celeritas::mysql_database_session::mysql_database_session(boost::asio::io_context& io_context, boost::asio::ssl::context* ssl_context)
    : io_context_{ io_context }, ssl_context_{ ssl_context }, connection_{ io_context }, params_{}, results_{}
{
}

celeritas::mysql_database_session::awaitable_type celeritas::mysql_database_session::async_connect(const std::string_view& host, const std::string_view& user, const std::string_view& password, const std::string_view& db_name)
{
    boost::asio::ip::tcp::resolver resolver(io_context_);
    auto endpoints = co_await resolver.async_resolve(host, boost::mysql::default_port_string, boost::asio::use_awaitable);

    params_ = std::make_unique<boost::mysql::handshake_params>(user, password, db_name);

    // 使用 boost::mysql::connect_params 来保存连接信息
    boost::mysql::connect_params connect_params;
    connect_params.server_address.emplace_host_and_port(std::string(host), *boost::mysql::default_port_string);
    connect_params.username = user;
    connect_params.password = password;
    connect_params.database = db_name;

    // 使用正确的参数调用 async_connect
    if (ssl_context_)
    {
        co_await connection_.async_connect(connect_params, *ssl_context_, boost::asio::use_awaitable);
    }
    else
    {
        co_await connection_.async_connect(connect_params, boost::asio::use_awaitable);
    }

    // 添加 co_return 语句以满足 awaitable<void> 的返回类型
    co_return;
}