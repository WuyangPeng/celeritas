#include "redis_sorted_set_commands.h"
#include "sorted_set_member_score.h"
#include "common/core/celeritas_error.h"

#include <string>

celeritas::redis_sorted_set_commands::redis_sorted_set_commands(redis_database_session& session) noexcept
    : base_type{ session }
{
}

celeritas::redis_sorted_set_commands::int_awaitable_type celeritas::redis_sorted_set_commands::async_add(const std::string& key, const sorted_set_member_score& member) const
{
    co_return co_await async_execute_command_return_int({ "ZADD", get_prefixed_key(key), std::to_string(member.get_score()), member.get_member() });
}

celeritas::redis_sorted_set_commands::int_awaitable_type celeritas::redis_sorted_set_commands::async_add_many(const std::string& key, const sorted_set_member_score_container& members) const
{
    if (members.empty())
    {
        co_return 0;
    }

    array_type command{ "ZADD", get_prefixed_key(key) };

    for (const auto& item : members)
    {
        command.emplace_back(std::to_string(item.get_score()));
        command.emplace_back(item.get_member());
    }

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_sorted_set_commands::int_awaitable_type celeritas::redis_sorted_set_commands::async_remove(const std::string& key, const sorted_set_member_score& member) const
{
    co_return co_await async_execute_command_return_int({ "ZREM", get_prefixed_key(key), member.get_member() });
}

celeritas::redis_sorted_set_commands::int_awaitable_type celeritas::redis_sorted_set_commands::async_remove_many(const std::string& key, const key_container& members) const
{
    array_type command{ "ZREM", get_prefixed_key(key) };
    command.insert(command.end(), members.begin(), members.end());

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_sorted_set_commands::optional_double_awaitable_type celeritas::redis_sorted_set_commands::async_score(const std::string& key, const std::string& member) const
{
    co_return co_await async_execute_command_return_optional_double({ "ZSCORE", get_prefixed_key(key), member });
}

celeritas::redis_sorted_set_commands::int_awaitable_type celeritas::redis_sorted_set_commands::async_sorted_set_cardinality(const std::string& key) const
{
    co_return co_await async_execute_command_return_int({ "ZCARD", get_prefixed_key(key) });
}

celeritas::redis_sorted_set_commands::optional_double_awaitable_type celeritas::redis_sorted_set_commands::async_increment_by(const std::string& key, const double increment, const std::string& member) const
{
    co_return co_await async_execute_command_return_optional_double({ "ZINCRBY", get_prefixed_key(key), std::to_string(increment), member });
}

celeritas::redis_sorted_set_commands::sorted_set_member_score_awaitable_type celeritas::redis_sorted_set_commands::async_range(const std::string& key, const int start, const int end, const bool with_scores) const
{
    array_type command{ "ZRANGE", get_prefixed_key(key), std::to_string(start), std::to_string(end) };
    if (with_scores)
    {
        command.emplace_back("WITHSCORES");
    }

    co_return co_await async_execute_command_return_sorted_set_member_score_type(command);
}

celeritas::redis_sorted_set_commands::sorted_set_member_score_awaitable_type celeritas::redis_sorted_set_commands::async_reverse_range(const std::string& key, const int start, const int end, const bool with_scores) const
{
    array_type command{ "ZREVRANGE", get_prefixed_key(key), std::to_string(start), std::to_string(end) };
    if (with_scores)
    {
        command.emplace_back("WITHSCORES");
    }

    co_return co_await async_execute_command_return_sorted_set_member_score_type(command);
}

celeritas::redis_sorted_set_commands::optional_int_awaitable_type celeritas::redis_sorted_set_commands::async_rank(const std::string& key, const std::string& member) const
{
    co_return co_await async_execute_command_return_optional_int({ "ZRANK", get_prefixed_key(key), member });
}

celeritas::redis_sorted_set_commands::optional_int_awaitable_type celeritas::redis_sorted_set_commands::async_reverse_rank(const std::string& key, const std::string& member) const
{
    co_return co_await async_execute_command_return_optional_int({ "ZREVRANK", key, member });
}
