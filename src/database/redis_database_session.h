#pragma once

#include "database_session.h"
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

        // 键操作

        // Redis是否健康
        [[nodiscard]] bool_awaitable_type is_health() override;

        // 删除键，返回删除个数
        [[nodiscard]] int_awaitable_type async_delete(const std::string& key);

        // 删除多个键是一个原子性操作，要么全部执行成功，要么全部不执行
        [[nodiscard]] int_awaitable_type async_delete_many(const container& keys);

        // 设置键的过期时间（秒）
        [[nodiscard]] bool_awaitable_type set_expire_seconds(const std::string& key, int expire_seconds);

        // 获取键的剩余生存时间 (TTL 命令)
        // 返回：>0 为剩余秒数，-1 为永不过期，-2 为键不存在
        [[nodiscard]] int_awaitable_type get_expire_seconds(const std::string& key);

        // 检查一个或多个键是否存在
        [[nodiscard]] bool_awaitable_type is_exists(const std::string& key);

        // 返回存在的个数
        [[nodiscard]] int_awaitable_type is_exists_many(const container& keys);

        // 重命名键。
        [[nodiscard]] bool_awaitable_type rename(const std::string& old_key, const std::string& new_key);

        // 获取键存储的数据类型
        [[nodiscard]] string_awaitable_type get_type(const std::string& key);

        [[nodiscard]] void_awaitable_type async_set(const std::string& key, const std::string& value, int expire_seconds = 0);

    private:
        using redis_context_unique_ptr = std::unique_ptr<redis_context>;

        [[nodiscard]] std::string get_prefixed_key(const std::string& key) const;

        [[nodiscard]] int_awaitable_type async_execute_command_return_int(const std::string& command) const;

        [[nodiscard]] void_awaitable_type async_execute_command_return_void(const std::string& command) const;

        [[nodiscard]] string_awaitable_type async_execute_command_return_string(const std::string& command) const;

        [[nodiscard]] std::string get_expire_seconds_command(int expire_seconds) const;

        redis_context_unique_ptr redis_context_;
        io_context_type& io_context_;

        std::string host_;
        int port_ = 0;
        std::string user_;
        std::string password_;
        std::string db_name_;
        int expire_seconds_;
    };
}
