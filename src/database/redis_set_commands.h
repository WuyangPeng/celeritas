#pragma once

#include "redis_commands.h"

namespace celeritas
{
    class redis_set_commands final : public redis_commands
    {
    public:
        using class_type = redis_set_commands;
        using base_type = redis_commands;

        explicit redis_set_commands(redis_database_session& session) noexcept;

        // 将一个成员添加到集合 key 中。
        // 返回：成功添加到集合中的新成员的数量 (不包括已存在的成员)。
        [[nodiscard]] int_awaitable_type async_add(const std::string& key, const std::string& member) const;

        // 将多个成员添加到集合 key 中。
        // 返回：成功添加到集合中的新成员的数量 (不包括已存在的成员)。
        [[nodiscard]] int_awaitable_type async_add_many(const std::string& key, const array_type& members) const;

        // 移除集合 key 中的一个成员。
        // 返回：成功移除的成员数量 (不包括不存在的成员)。
        [[nodiscard]] int_awaitable_type async_remove(const std::string& key, const std::string& member) const;

        // 移除集合 key 中的多个成员。
        // 返回：成功移除的成员数量 (不包括不存在的成员)。
        [[nodiscard]] int_awaitable_type async_remove_many(const std::string& key, const array_type& members) const;

        // 获取集合 key 的成员数量。
        // 返回：集合的基数 (成员数量)。
        [[nodiscard]] int_awaitable_type async_set_cardinality(const std::string& key) const;

        // 判断 member 元素是否集合 key 的成员。
        // 返回：true 表示成员存在，false 表示不存在或 key 不存在。
        [[nodiscard]] bool_awaitable_type async_set_is_member(const std::string& key, const std::string& member) const;

        // 返回集合 key 中的所有成员。
        // 返回：集合中所有成员。
        [[nodiscard]] array_awaitable_type async_set_members(const std::string& key) const;

        // 返回给定所有集合的并集。
        // keys: 包含要计算并集的集合 key 的列表。
        // 返回：所有集合的并集成员。
        [[nodiscard]] array_awaitable_type async_set_union(const array_type& keys) const;

        // 返回给定所有集合的交集。
        // keys: 包含要计算交集的集合 key 的列表。
        // 返回：所有集合的交集成员。
        [[nodiscard]] array_awaitable_type async_set_inter(const array_type& keys) const;

        // 返回第一个集合与后面所有集合的差集。
        // keys: 包含要计算差集的集合 key 的列表。
        // 返回：差集成员的 std::set 容器。
        [[nodiscard]] array_awaitable_type async_set_diff(const array_type& keys) const;
    };
}