#pragma once

#include "database_session.h"
#include "database/detail/mysql_parameter.h"

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
        using results_type = boost::mysql::results;
        using results_awaitable_type = boost::asio::awaitable<results_type>;

        mysql_database_session(const std::string& host,
                               int port,
                               const std::string& user,
                               const std::string& password,
                               const std::string& uri,
                               const std::string& db_name,
                               int expire_seconds,
                               const any_io_executor& any_io_executor,
                               ssl_io_context_type* ssl_context = nullptr);

        ~mysql_database_session() noexcept override;

        mysql_database_session(const mysql_database_session& rhs) noexcept = delete;

        mysql_database_session& operator=(const mysql_database_session& rhs) noexcept = delete;

        mysql_database_session(mysql_database_session&& rhs) noexcept = delete;

        mysql_database_session& operator=(mysql_database_session&& rhs) noexcept = delete;

        // 异步连接到数据库
        [[nodiscard]] void_awaitable_type async_connect();

        // 异步执行查询，返回结果集
        [[nodiscard]] results_awaitable_type async_query(const std::string& sql);

        [[nodiscard]] bool_awaitable_type is_health() override;

        [[nodiscard]] void_awaitable_type execute_changes(const database_entity_change_const_shared_ptr& database, int expiration_time) override;

        [[nodiscard]] database_entity_change_awaitable_type select_one(const database_entity_change_const_shared_ptr& database, const database_field_container& field_name_container) override;

        [[nodiscard]] result_container_awaitable_type select_all(const database_entity_change_const_shared_ptr& database, const database_field_container& field_name_container) override;

    private:
        using connection_type = boost::mysql::any_connection;
        using error_code_type = boost::mysql::error_code;
        using field_view_type = boost::mysql::field_view;
        using row_view_type = boost::mysql::row_view;

        [[nodiscard]] static connection_type get_any_connection(const any_io_executor& any_io_executor, ssl_io_context_type* ssl_context);

        [[nodiscard]] results_awaitable_type async_execute_query(const std::string& sql);

        [[nodiscard]] results_awaitable_type async_handle_and_retry(const std::string& sql, const error_code_type& error_code);

        [[nodiscard]] static database_entity_change populate_database_from_row(const database_entity_change_const_shared_ptr& database, const database_field_container& field_name_container, const row_view_type& row);

        connection_type connection_;
        mysql_parameter mysql_parameter_;
    };
}
