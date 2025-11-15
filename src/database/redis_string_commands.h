#pragma once

#include "database_fwd.h"
#include "redis_commands.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class redis_string_commands final : public redis_commands
    {
    public:
        using class_type = redis_string_commands;
        using base_type = redis_commands;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using string_awaitable_type = boost::asio::awaitable<std::string>;
        using optional_string = std::optional<std::string>;
        using optional_string_awaitable_type = boost::asio::awaitable<optional_string>;
        using int_awaitable_type = boost::asio::awaitable<int>;
        using bool_awaitable_type = boost::asio::awaitable<bool>;
        using array_type = std::vector<std::string>;
        using array_awaitable_type = boost::asio::awaitable<array_type>;

        explicit redis_string_commands(redis_database_session& session) noexcept;

        // 设置键值对，可选过期时间
        [[nodiscard]] bool_awaitable_type async_set(const std::string& key, const std::string& value, int expire_seconds = 0) const;

        // 设置键值对，仅当键不存在时才设置
        // 返回：true 表示设置成功 (键不存在)，false 表示未设置 (键已存在)
        // 核心用途：分布式锁和互斥。
        // 过期时间必须大于零。
        [[nodiscard]] bool_awaitable_type async_set_not_exists(const std::string& key, const std::string& value, int expire_seconds = 0) const;

        // 设置键值对，仅当键存在时才设置
        // 返回：true 表示设置成功 (键已存在)，false 表示未设置 (键不存在)
        [[nodiscard]] bool_awaitable_type async_set_exists(const std::string& key, const std::string& value, int expire_seconds = 0) const;

        // 批量设置多个键值对
        [[nodiscard]] bool_awaitable_type async_set_many(const key_value_container& key_values) const;

        // 将值追加到key的末尾
        // 返回：追加操作后字符串的总长度
        [[nodiscard]] int_awaitable_type async_append(const std::string& key, const std::string& value) const;

        // 将键存储的值增加指定的整数
        [[nodiscard]] int_awaitable_type async_increment_by(const std::string& key, int increment) const;

        // 将键存储的值增加 1
        [[nodiscard]] int_awaitable_type async_increment(const std::string& key) const;

        // 将键存储的值减少指定的整数
        [[nodiscard]] int_awaitable_type async_decrement_by(const std::string& key, int decrement) const;

        // 将键存储的值减少 1
        [[nodiscard]] int_awaitable_type async_decrement(const std::string& key) const;

        // 获取键的值
        // 返回：如果键存在则返回其值，否则返回 std::nullopt
        [[nodiscard]] optional_string_awaitable_type async_get(const std::string& key) const;

        // 批量获取多个键的值
        // 返回：std::vector<std::string>，其中不存在的键对应元素为 ""
        [[nodiscard]] array_awaitable_type async_get_many(const key_container& keys) const;

        // 获取并设置键值对
        // 返回：原始值，如果键不存在则返回 std::nullopt
        [[nodiscard]] optional_string_awaitable_type async_get_set(const std::string& key, const std::string& value) const;
    };
}