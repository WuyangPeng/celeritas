#pragma once

#include "database_fwd.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class redis_key_commands
    {
    public:
        using class_tye = redis_key_commands;
        using int_awaitable_type = boost::asio::awaitable<int>;
        using string_awaitable_type = boost::asio::awaitable<std::string>;
        using bool_awaitable_type = boost::asio::awaitable<bool>;
        using container = std::vector<std::string>;

        explicit redis_key_commands(redis_database_session& session) noexcept;

        // 删除键，返回删除个数
        [[nodiscard]] int_awaitable_type async_delete(const std::string& key) const;

        // 删除多个键是一个原子性操作，要么全部执行成功，要么全部不执行
        [[nodiscard]] int_awaitable_type async_delete_many(const container& keys) const;

        // 设置键的过期时间（秒）
        [[nodiscard]] bool_awaitable_type set_expire_seconds(const std::string& key, int expire_seconds) const;

        // 获取键的剩余生存时间 (TTL 命令)
        // 返回：>0 为剩余秒数，-1 为永不过期，-2 为键不存在
        [[nodiscard]] int_awaitable_type get_expire_seconds(const std::string& key) const;

        // 检查一个或多个键是否存在
        [[nodiscard]] bool_awaitable_type is_exists(const std::string& key) const;

        // 返回存在的个数
        [[nodiscard]] int_awaitable_type is_exists_many(const container& keys) const;

        // 重命名键
        [[nodiscard]] bool_awaitable_type rename(const std::string& old_key, const std::string& new_key) const;

        // 获取键存储的数据类型
        [[nodiscard]] string_awaitable_type get_type(const std::string& key) const;

    private:
        [[nodiscard]] std::string get_keys_command(const container& keys) const;

    private:
        redis_database_session& session_;
    };
}