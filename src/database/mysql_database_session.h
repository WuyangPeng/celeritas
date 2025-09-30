#pragma once

#include "database_session.h"

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/mysql.hpp>

namespace celeritas
{
    // 数据库会话类，代表一个数据库连接
    class mysql_database_session : public database_session
    {
    public:
        using class_type = mysql_database_session;
        using base_type = database_session;
        using awaitable_type = boost::asio::awaitable<void>;
        using results_type = boost::asio::awaitable<boost::mysql::results>;

        mysql_database_session(const std::string_view& host,
                               uint16_t port,
                               const std::string_view& user,
                               const std::string_view& password,
                               const std::string_view& uri,
                               const std::string_view& db_name,
                               boost::asio::io_context& io_context,
                               boost::asio::ssl::context* ssl_context = nullptr);

        ~mysql_database_session() noexcept override;

        mysql_database_session(const mysql_database_session& rhs) noexcept = default;

        mysql_database_session& operator=(const mysql_database_session& rhs) noexcept = default;

        mysql_database_session(mysql_database_session&& rhs) noexcept = default;

        mysql_database_session& operator=(mysql_database_session&& rhs) noexcept = default;

        // 异步连接到数据库
        [[nodiscard]] awaitable_type async_connect();

        // 异步执行查询，返回结果集
        [[nodiscard]] results_type async_query(const std::string_view& sql);

    private:
        using connection_type = boost::mysql::any_connection;

        [[nodiscard]] static connection_type get_any_connection(boost::asio::io_context& io_context, boost::asio::ssl::context* ssl_context);

        [[nodiscard]] results_type async_execute_query(const std::string_view& sql);

        [[nodiscard]] results_type async_handle_and_retry(const std::string_view& sql, const boost::system::error_code& error_code);

        const std::string host_;
        uint16_t port_;
        const std::string user_;
        const std::string password_;
        const std::string db_name_;
        connection_type connection_;
    };
}
