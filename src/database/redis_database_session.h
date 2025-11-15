#pragma once

#include "database_session.h"
#include "redis_hash_commands.h"
#include "redis_key_commands.h"
#include "redis_list_commands.h"
#include "redis_set_commands.h"
#include "redis_sorted_set_commands.h"
#include "redis_string_commands.h"
#include "detail/redis_context.h"

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
        using void_awaitable_type = boost::asio::awaitable<void>;
        using int_awaitable_type = boost::asio::awaitable<int>;
        using optional_double = std::optional<double>;
        using optional_double_awaitable_type = boost::asio::awaitable<optional_double>;
        using optional_string = std::optional<std::string>;
        using optional_string_awaitable_type = boost::asio::awaitable<optional_string>;
        using io_context_type = boost::asio::io_context;
        using array_type = std::vector<std::string>;
        using array_type_awaitable_type = boost::asio::awaitable<array_type>;
        using map_type = std::map<std::string, std::string>;
        using map_type_awaitable_type = boost::asio::awaitable<map_type>;
        using optional_int = std::optional<int>;
        using optional_int_awaitable_type = boost::asio::awaitable<optional_int>;

        redis_database_session(const std::string_view& host,
                               int port,
                               const std::string_view& user,
                               const std::string_view& password,
                               const std::string_view& uri,
                               const std::string_view& db_name,
                               int expire_seconds,
                               io_context_type& io_context);

        ~redis_database_session() noexcept override = default;

        redis_database_session(const redis_database_session& rhs) noexcept = delete;

        redis_database_session& operator=(const redis_database_session& rhs) noexcept = delete;

        redis_database_session(redis_database_session&& rhs) noexcept = delete;

        redis_database_session& operator=(redis_database_session&& rhs) noexcept = delete;

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

        [[nodiscard]] std::string get_prefixed_key(const std::string& key) const;

        [[nodiscard]] std::string get_expire_seconds_command(int expire_seconds) const;

        [[nodiscard]] int_awaitable_type async_execute_command_return_int(const std::string& command) const;

        [[nodiscard]] void_awaitable_type async_execute_command_return_void(const std::string& command) const;

        [[nodiscard]] optional_string_awaitable_type async_execute_command_return_optional_string(const std::string& command) const;

        [[nodiscard]] array_type_awaitable_type async_execute_command_return_array_type(const std::string& command) const;

        [[nodiscard]] map_type_awaitable_type async_execute_command_return_map_type(const std::string& command) const;

        [[nodiscard]] optional_double_awaitable_type async_execute_command_return_optional_double(const std::string& command) const;

        [[nodiscard]] int_awaitable_type async_execute_command_return_optional_int(const std::string& command) const;

    private:
        using redis_context_unique_ptr = std::unique_ptr<redis_context>;

        void check_initialized() const;

        void do_is_health();

        redis_context_unique_ptr redis_context_;
        io_context_type& io_context_;

        std::string host_;
        int port_ = 0;
        std::string user_;
        std::string password_;
        std::string db_name_;
        int expire_seconds_ = 0;

        redis_key_commands redis_key_commands_;
        redis_string_commands redis_string_commands_;
        redis_hash_commands redis_hash_commands_;
        redis_list_commands redis_list_commands_;
        redis_set_commands redis_set_commands_;
        redis_sorted_set_commands redis_sorted_set_commands_;
    };
}
