#pragma once

#include "redis_commands.h"

namespace celeritas
{
    class redis_key_commands final : public redis_commands
    {
    public:
        using class_tye = redis_key_commands;
        using base_type = redis_commands;

        explicit redis_key_commands(redis_database_session& session) noexcept;

        // 删除键，返回删除个数
        [[nodiscard]] int_awaitable_type async_delete(const std::string& key) const;

        // 删除多个键是一个原子性操作，要么全部执行成功，要么全部不执行
        [[nodiscard]] int_awaitable_type async_delete_many(const key_container& keys) const;

        // 设置键的过期时间（秒）
        [[nodiscard]] bool_awaitable_type async_set_expire_seconds(const std::string& key, int expire_seconds) const;

        // 获取键的剩余生存时间 (TTL 命令)
        // 返回：>0 为剩余秒数，-1 为永不过期，-2 为键不存在
        [[nodiscard]] int_awaitable_type async_get_expire_seconds(const std::string& key) const;

        // 检查一个或多个键是否存在
        [[nodiscard]] bool_awaitable_type async_is_exists(const std::string& key) const;

        // 返回存在的个数
        [[nodiscard]] int_awaitable_type async_is_exists_many(const key_container& keys) const;

        // 重命名键
        [[nodiscard]] bool_awaitable_type async_rename(const std::string& old_key, const std::string& new_key) const;

        // 获取键存储的数据类型
        [[nodiscard]] string_awaitable_type async_get_type(const std::string& key) const;

        // 使用SCAN命令迭代键空间，查找与给定模式匹配的键。
        // 此函数只执行一次 SCAN 命令。
        // 返回：std::pair<cursor, keys>，其中 cursor 是下一次迭代的游标。
        [[nodiscard]] scan_result_awaitable_type async_scan(const std::string& pattern, int cursor, int count) const;

        // 迭代执行SCAN命令
        [[nodiscard]] array_awaitable_type async_scan_all(const std::string& pattern) const;
    };
}