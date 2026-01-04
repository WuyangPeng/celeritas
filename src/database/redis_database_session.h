#pragma once

#include "database_session.h"
#include "redis_hash_commands.h"
#include "redis_key_commands.h"
#include "redis_list_commands.h"
#include "redis_set_commands.h"
#include "redis_sorted_set_commands.h"
#include "redis_string_commands.h"
#include "scan_result.h"
#include "detail/redis_context.h"
#include "detail/redis_parameter.h"
#include "detail/redis_reply.h"

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <memory>
#include <string>

namespace celeritas
{
    class redis_database_session final : public database_session
    {
    public:
        using class_type = redis_database_session;
        using base_type = database_session;
        using io_context_type = boost::asio::io_context;
        using int_awaitable_type = boost::asio::awaitable<int>;
        using optional_int = std::optional<int>;
        using optional_int_awaitable_type = boost::asio::awaitable<optional_int>;
        using optional_double = std::optional<double>;
        using optional_double_awaitable_type = boost::asio::awaitable<optional_double>;
        using optional_string = std::optional<std::string>;
        using optional_string_awaitable_type = boost::asio::awaitable<optional_string>;
        using array_type = std::vector<std::string>;
        using array_awaitable_type = boost::asio::awaitable<array_type>;
        using map_type = std::map<std::string, std::string>;
        using map_awaitable_type = boost::asio::awaitable<map_type>;
        using optional_map_type = std::optional<map_type>;
        using optional_map_awaitable_type = boost::asio::awaitable<optional_map_type>;
        using scan_result_awaitable_type = boost::asio::awaitable<scan_result>;

        redis_database_session(std::string_view host,
                               int port,
                               std::string_view user,
                               std::string_view password,
                               std::string_view uri,
                               std::string_view db_name,
                               int expire_seconds,
                               const any_io_executor& any_io_executor);

        // 异步连接到Redis
        [[nodiscard]] void_awaitable_type async_connect();

        // Redis是否健康
        [[nodiscard]] bool_awaitable_type is_health() override;

        // 键操作
        [[nodiscard]] redis_key_commands& get_redis_key_commands();

        // 字符串操作
        [[nodiscard]] redis_string_commands& get_redis_string_commands();

        // 哈希操作
        [[nodiscard]] redis_hash_commands& get_redis_hash_commands();

        // 列表操作
        [[nodiscard]] redis_list_commands& get_redis_list_commands();

        // 集合操作
        [[nodiscard]] redis_set_commands& get_redis_set_commands();

        // 有序集合操作
        [[nodiscard]] redis_sorted_set_commands& get_redis_sorted_set_commands();

        // 辅助函数
        [[nodiscard]] std::string get_prefixed_key(const std::string& key) const;

        [[nodiscard]] array_type get_expire_seconds_command(int expire_seconds) const;

        [[nodiscard]] int_awaitable_type async_execute_command_return_int(const array_type& command) const;

        [[nodiscard]] void_awaitable_type async_execute_command_return_void(const array_type& command) const;

        [[nodiscard]] optional_string_awaitable_type async_execute_command_return_optional_string(const array_type& command) const;

        [[nodiscard]] array_awaitable_type async_execute_command_return_array_type(const array_type& command) const;

        [[nodiscard]] map_awaitable_type async_execute_command_return_map_type(const array_type& command) const;

        [[nodiscard]] optional_double_awaitable_type async_execute_command_return_optional_double(const array_type& command) const;

        [[nodiscard]] optional_int_awaitable_type async_execute_command_return_optional_int(const array_type& command) const;

        [[nodiscard]] optional_map_awaitable_type async_execute_command_return_optional_map_type(const array_type& command) const;

        [[nodiscard]] scan_result_awaitable_type async_execute_command_return_scan_result(const array_type& command) const;

        [[nodiscard]] void_awaitable_type execute_changes(const database_entity_change_const_shared_ptr& database, int expiration_time) override;

        [[nodiscard]] database_entity_change_awaitable_type select_one(const database_entity_change_const_shared_ptr& database, const database_field_container& field_name_container) override;

        [[nodiscard]] result_container_awaitable_type select_all(const database_entity_change_const_shared_ptr& database, const database_field_container& field_name_container) override;

    private:
        using redis_context_unique_ptr = std::unique_ptr<redis_context>;
        using redis_reply_unique_ptr = std::unique_ptr<redis_reply>;
        using redis_reply_awaitable_type = boost::asio::awaitable<redis_reply_unique_ptr>;

        void check_initialized() const;

        void do_is_health() const;

        [[nodiscard]] redis_reply_awaitable_type async_execute_command_return_reply(const array_type& command) const;

        [[nodiscard]] void_awaitable_type save_database(const database_entity_change_const_shared_ptr& database, int expiration_time) const;

        [[nodiscard]] void_awaitable_type delete_database(const database_entity_change_const_shared_ptr& database) const;

        [[nodiscard]] database_entity_change_awaitable_type select_one(const std::string& key, const database_entity_change_const_shared_ptr& database, const database_field_container& field_name_container) const;

        any_io_executor any_io_executor_;
        redis_context_unique_ptr redis_context_;

        redis_parameter redis_parameter_;
        redis_key_commands redis_key_commands_;
        redis_string_commands redis_string_commands_;
        redis_hash_commands redis_hash_commands_;
        redis_list_commands redis_list_commands_;
        redis_set_commands redis_set_commands_;
        redis_sorted_set_commands redis_sorted_set_commands_;
    };
}
