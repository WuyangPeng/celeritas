#include "redis_sorted_set_commands.h"
#include "redis_database_session.h"
#include "sorted_set_member_score.h"
#include "common/celeritas_error.h"

#include <string>

celeritas::redis_sorted_set_commands::redis_sorted_set_commands(redis_database_session& session) noexcept
    : base_type{ session }
{
}

celeritas::redis_sorted_set_commands::int_awaitable_type celeritas::redis_sorted_set_commands::async_add(const std::string& key, const sorted_set_member_score& member) const
{
    const auto command = "ZADD " + get_prefixed_key(key) + " " + std::to_string(member.get_score()) + " " + get_quoted_value_command(member.get_member());

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_sorted_set_commands::int_awaitable_type celeritas::redis_sorted_set_commands::async_add_many(const std::string& key, const sorted_set_member_score_container& members) const
{
    if (members.empty())
    {
        co_return 0;
    }

    const auto prefixed_key = get_prefixed_key(key);

    auto command = "ZADD " + prefixed_key;

    for (const auto& item : members)
    {
        command += " " + std::to_string(item.get_score()) + " \"" + item.get_member() + "\"";
    }

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_sorted_set_commands::int_awaitable_type celeritas::redis_sorted_set_commands::async_remove(const std::string& key, const sorted_set_member_score& member) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto decrement_command = "ZREM " + prefixed_key;

    co_return co_await async_execute_command_return_int(decrement_command);
}

celeritas::redis_sorted_set_commands::int_awaitable_type celeritas::redis_sorted_set_commands::async_remove_many(const std::string& key, const key_container& members) const
{
    const auto decrement_command = "ZREM " + get_keys_command(members);

    co_return co_await async_execute_command_return_int(decrement_command);
}

celeritas::redis_sorted_set_commands::optional_double_awaitable_type celeritas::redis_sorted_set_commands::async_score(const std::string& key, const std::string& member) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = std::string("ZSCORE ") + prefixed_key + " \"" + member + "\"";

    co_return co_await async_execute_command_return_optional_double(command);
}

celeritas::redis_sorted_set_commands::int_awaitable_type celeritas::redis_sorted_set_commands::async_sorted_set_cardinality(const std::string& key) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = std::string("ZCARD ") + prefixed_key;

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_sorted_set_commands::optional_double_awaitable_type celeritas::redis_sorted_set_commands::async_increment_by(const std::string& key, double increment, const std::string& member) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = std::string("ZINCRBY ") + prefixed_key + " " + std::to_string(increment) + " \"" + member + "\"";

    co_return co_await async_execute_command_return_optional_double(command);
}

celeritas::redis_sorted_set_commands::sorted_set_member_score_container celeritas::redis_sorted_set_commands::convert_array_to_scores(const array_type& array_result, bool with_scores) const
{
    sorted_set_member_score_container result{};

    if (array_result.empty())
    {
        return result;
    }

    if (!with_scores)
    {
        result.reserve(array_result.size());
        for (const auto& member : array_result)
        {
            result.emplace_back(member, 0.0);
        }
        return result;
    }

    if (array_result.size() % 2 != 0)
    {
        throw celeritas_error("ZRANGE/ZREVRANGE with scores returned an array with odd number of elements.");
    }

    result.reserve(array_result.size() / 2);
    for (auto i = 0; i < array_result.size(); i += 2)
    {
        const auto& member = array_result[i];
        const auto& score_str = array_result[i + 1];

        try
        {
            result.emplace_back(member, std::stod(score_str));
        }
        catch (const std::exception& e)
        {
            throw celeritas_error("ZRANGE/ZREVRANGE: failed to convert score string to double: " + score_str + ". Error: " + e.what());
        }
    }
    return result;
}

celeritas::redis_sorted_set_commands::sorted_set_member_score_awaitable_type celeritas::redis_sorted_set_commands::async_range(const std::string& key, const int start, const int end, const bool with_scores) const
{
    const auto prefixed_key = get_prefixed_key(key);

    auto command = "ZRANGE " + prefixed_key + " " + std::to_string(start) + " " + std::to_string(end);
    if (with_scores)
    {
        command += " WITHSCORES";
    }

    const auto array_result = co_await async_execute_command_return_array_type(command);

    co_return convert_array_to_scores(array_result, with_scores);
}

celeritas::redis_sorted_set_commands::sorted_set_member_score_awaitable_type celeritas::redis_sorted_set_commands::async_reverse_range(const std::string& key, const int start, const int end, const bool with_scores) const
{
    const auto prefixed_key = get_prefixed_key(key);

    auto command = "ZREVRANGE " + prefixed_key + " " + std::to_string(start) + " " + std::to_string(end);
    if (with_scores)
    {
        command += " WITHSCORES";
    }

    const auto array_result = co_await async_execute_command_return_array_type(command);

    co_return convert_array_to_scores(array_result, with_scores);
}

celeritas::redis_sorted_set_commands::optional_int_awaitable_type celeritas::redis_sorted_set_commands::async_rank(const std::string& key, const std::string& member) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = std::string("ZRANK ") + prefixed_key + " \"" + member + "\"";

    co_return co_await async_execute_command_return_optional_int(command);
}

celeritas::redis_sorted_set_commands::optional_int_awaitable_type celeritas::redis_sorted_set_commands::async_reverse_rank(const std::string& key, const std::string& member) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = std::string("ZREVRANK ") + prefixed_key + " \"" + member + "\"";

    co_return co_await async_execute_command_return_optional_int(command);
}