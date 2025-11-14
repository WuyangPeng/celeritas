#pragma once

#include "database_session.h"

#include <boost/mysql.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/ssl.hpp>

namespace celeritas
{
    class mysql_database_session final : public database_session
    {
    public:
        using class_type = mysql_database_session;
        using base_type = database_session;
        using io_context_type = boost::asio::io_context;
        using ssl_io_context_type = boost::asio::ssl::context;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using results_type = boost::mysql::results;
        using results_awaitable_type = boost::asio::awaitable<results_type>;

        mysql_database_session(const std::string_view& host,
                               int port,
                               const std::string_view& user,
                               const std::string_view& password,
                               const std::string_view& uri,
                               const std::string_view& db_name,
                               int expire_seconds,
                               io_context_type& io_context,
                               ssl_io_context_type* ssl_context = nullptr);

        ~mysql_database_session() noexcept override;

        mysql_database_session(const mysql_database_session& rhs) noexcept = delete;

        mysql_database_session& operator=(const mysql_database_session& rhs) noexcept = delete;

        mysql_database_session(mysql_database_session&& rhs) noexcept = delete;

        mysql_database_session& operator=(mysql_database_session&& rhs) noexcept = delete;

        // 异步连接到数据库
        [[nodiscard]] void_awaitable_type async_connect();

        // 异步执行查询，返回结果集
        [[nodiscard]] results_awaitable_type async_query(const std::string_view& sql);

        [[nodiscard]] bool_awaitable_type is_health() override;

    private:
        using connection_type = boost::mysql::any_connection;
        using error_code_type = boost::mysql::error_code;

        [[nodiscard]] static connection_type get_any_connection(io_context_type& io_context, ssl_io_context_type* ssl_context);

        [[nodiscard]] results_awaitable_type async_execute_query(const std::string_view& sql);

        [[nodiscard]] results_awaitable_type async_handle_and_retry(const std::string_view& sql, const error_code_type& error_code);

        const std::string host_;
        int port_;
        const std::string user_;
        const std::string password_;
        const std::string db_name_;
        connection_type connection_;
    };
}
