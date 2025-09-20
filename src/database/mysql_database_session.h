#pragma once

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/mysql.hpp>

#include <string>

namespace celeritas
{
    // 数据库会话类，代表一个数据库连接
    class mysql_database_session
    {
    public:
        using class_type = mysql_database_session;
        using awaitable_type = boost::asio::awaitable<void>;
        using rows_view_type = boost::asio::awaitable<boost::mysql::rows_view>;

        explicit mysql_database_session(boost::asio::io_context& io_context, boost::asio::ssl::context* ssl_context = nullptr);

        // 异步连接到数据库
        [[nodiscard]] awaitable_type async_connect(const std::string_view& host,
                                                   uint16_t port,
                                                   const std::string_view& user,
                                                   const std::string_view& password,
                                                   const std::string_view& db_name);

        // 异步执行查询，返回结果集
        [[nodiscard]] rows_view_type async_query(const std::string_view& sql);

    private:
        using connection_type = boost::mysql::any_connection;

        [[nodiscard]] static connection_type get_any_connection(boost::asio::io_context& io_context, boost::asio::ssl::context* ssl_context);

        connection_type connection_;
    };
}
