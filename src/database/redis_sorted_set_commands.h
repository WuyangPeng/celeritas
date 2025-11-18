#pragma once

#include "redis_commands.h"

namespace celeritas
{
    class redis_sorted_set_commands final : public redis_commands
    {
    public:
        using class_type = redis_sorted_set_commands;
        using base_type = redis_commands;
        using sorted_set_member_score_container = std::vector<sorted_set_member_score>;
        using sorted_set_member_score_awaitable_type = boost::asio::awaitable<sorted_set_member_score_container>;

        explicit redis_sorted_set_commands(redis_database_session& session) noexcept;

        // 将一个成员及其 score 值加入到有序集合中。
        // 返回：成功添加到有序集合中的成员数量。
        [[nodiscard]] int_awaitable_type async_add(const std::string& key, const sorted_set_member_score& member) const;

        // 将多个成员及其 score 值加入到有序集合中。
        // 返回：成功添加到有序集合中的成员数量。
        [[nodiscard]] int_awaitable_type async_add_many(const std::string& key, const sorted_set_member_score_container& members) const;

        // 移除有序集合 key 中的一个成员。
        // 返回：成功移除的成员数量。
        [[nodiscard]] int_awaitable_type async_remove(const std::string& key, const sorted_set_member_score& member) const;

        // 移除有序集合 key 中的多个成员。
        // 返回：成功移除的成员数量。
        [[nodiscard]] int_awaitable_type async_remove_many(const std::string& key, const key_container& members) const;

        // 获取有序集合中指定成员的 score 值。
        // 返回：成员的 score，如果 key 或 member 不存在则返回 std::nullopt。
        [[nodiscard]] optional_double_awaitable_type async_score(const std::string& key, const std::string& member) const;

        // 获取有序集合的成员数量。
        // 返回：有序集合的基数 (成员数量)。
        [[nodiscard]] int_awaitable_type async_sorted_set_cardinality(const std::string& key) const;

        // 有序集合中对指定成员的分数进行递增。
        // 返回：递增后的新 score 值。
        [[nodiscard]] optional_double_awaitable_type async_increment_by(const std::string& key, double increment, const std::string& member) const;

        // 通过索引区间返回有序集合指定区间内的成员 (升序)。
        // with_scores: 是否在结果中包含 score。
        // 返回：成员和分数列表。
        [[nodiscard]] sorted_set_member_score_awaitable_type async_range(const std::string& key, int start, int end, bool with_scores = false) const;

        // 通过索引区间返回有序集合中指定区间内的成员 (降序)。
        // with_scores: 是否在结果中包含 score。
        // 返回：成员和分数列表。
        [[nodiscard]] sorted_set_member_score_awaitable_type async_reverse_range(const std::string& key, int start, int end, bool with_scores = false) const;

        // 返回有序集合中指定成员的排名 (升序，从 0 开始)。
        // 返回：成员的排名，如果成员不存在则返回 std::nullopt。
        [[nodiscard]] optional_int_awaitable_type async_rank(const std::string& key, const std::string& member) const;

        // 返回有序集合中指定成员的排名 (降序，从 0 开始)。
        // 返回：成员的排名，如果成员不存在则返回 std::nullopt。
        [[nodiscard]] optional_int_awaitable_type async_reverse_rank(const std::string& key, const std::string& member) const;

    private:
        [[nodiscard]] static sorted_set_member_score_container convert_array_to_members(const array_type& array_result, bool with_scores);

        [[nodiscard]] static sorted_set_member_score_container convert_array_to_members_with_scores(const array_type& array_result);

        [[nodiscard]] static sorted_set_member_score convert_array_to_members_with_scores(const array_type& array_result, int index);

        [[nodiscard]] static sorted_set_member_score_container convert_array_to_members_without_scores(const array_type& array_result);
    };
}