#pragma once

#include "redis_commands.h"

namespace celeritas
{
    class redis_hash_commands final : public redis_commands
    {
    public:
        using class_type = redis_hash_commands;
        using base_type = redis_commands;

        explicit redis_hash_commands(redis_database_session& session) noexcept;

        // 设置哈希表 key 中指定字段 field 的值 value。
        // 如果 key 不存在，Redis 会自动创建 Hash Key。如果 key 存在但不是 Hash 类型，命令会失败。
        // 返回 1 如果 field 是一个新字段，返回 0 如果 field 已经存在
        [[nodiscard]] int_awaitable_type async_set(const std::string& key, const std::string& field, const std::string& value) const;

        // 批量设置哈希表 key 中多个字段的值。
        // key_value_container 包含 (field, value) 对。
        // 返回的是新字段的数量
        [[nodiscard]] int_awaitable_type async_set_many(const std::string& key, const key_value_container& field_values) const;

        // 删除哈希表中一个字段。
        [[nodiscard]] int_awaitable_type async_delete(const std::string& key, const std::string& field) const;

        // 删除哈希表中多个字段。
        [[nodiscard]] int_awaitable_type async_delete_many(const std::string& key, const array_type& fields) const;

        // 为哈希表 key 中的字段 field 的值加上增量 increment。
        // 返回：执行加法操作之后 field 的值。
        [[nodiscard]] int_awaitable_type async_increment_by(const std::string& key, const std::string& field, int increment) const;

        // 获取哈希表 key 中指定字段 field 的值。
        // 返回：如果字段存在则返回其值，否则返回 std::nullopt。
        [[nodiscard]] optional_string_awaitable_type async_get(const std::string& key, const std::string& field) const;

        // 批量获取多个字段的值。
        [[nodiscard]] array_awaitable_type async_get_many(const std::string& key, const array_type& fields) const;

        // 获取哈希表中的所有字段名。
        [[nodiscard]] array_awaitable_type async_get_fields(const std::string& key) const;

        // 批量获取哈希表 key 中多个字段的值。
        // 返回：std::map<std::string, std::string>，其中不存在的字段不会被包含。
        [[nodiscard]] map_awaitable_type async_get_all(const std::string& key) const;

        // 批量获取哈希表 key 中多个字段的值。
        // 使用最终的key
        [[nodiscard]] map_awaitable_type async_get_all_by_real_key(const std::string& key) const;
    };
}