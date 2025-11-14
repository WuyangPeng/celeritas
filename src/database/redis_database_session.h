#pragma once

#include "database_session.h"
#include "redis_key_commands.h"
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
        using string_awaitable_type = boost::asio::awaitable<std::string>;
        using io_context_type = boost::asio::io_context;
        using container = std::vector<std::string>;

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

        [[nodiscard]] void_awaitable_type async_set(const std::string& key, const std::string& value, int expire_seconds = 0);

        [[nodiscard]] std::string get_prefixed_key(const std::string& key) const;

        [[nodiscard]] int_awaitable_type async_execute_command_return_int(const std::string& command) const;

        [[nodiscard]] void_awaitable_type async_execute_command_return_void(const std::string& command) const;

        [[nodiscard]] string_awaitable_type async_execute_command_return_string(const std::string& command) const;

    private:
        using redis_context_unique_ptr = std::unique_ptr<redis_context>;

        [[nodiscard]] std::string get_expire_seconds_command(int expire_seconds) const;

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
    };
}
