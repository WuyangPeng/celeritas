#include "redis_list_commands.h"
#include "common/core/celeritas_error.h"

celeritas::redis_list_commands::redis_list_commands(redis_database_session& session) noexcept
    : base_type{ session }
{
}

celeritas::redis_list_commands::int_awaitable_type celeritas::redis_list_commands::async_left_push(const std::string& key, const std::string& value) const
{
    co_return co_await async_execute_command_return_int({ "LPUSH", get_prefixed_key(key), value });
}

celeritas::redis_list_commands::int_awaitable_type celeritas::redis_list_commands::async_left_push_many(const std::string& key, const key_container& values) const
{
    if (values.empty())
    {
        co_return 0;
    }

    array_type command{ "LPUSH", get_prefixed_key(key) };
    command.insert(command.end(), values.begin(), values.end());

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_list_commands::int_awaitable_type celeritas::redis_list_commands::async_right_push(const std::string& key, const std::string& value) const
{
    co_return co_await async_execute_command_return_int({ "RPUSH", get_prefixed_key(key), value });
}

celeritas::redis_list_commands::int_awaitable_type celeritas::redis_list_commands::async_right_push_many(const std::string& key, const key_container& values) const
{
    if (values.empty())
    {
        co_return 0;
    }

    array_type command{ "RPUSH", get_prefixed_key(key) };
    command.insert(command.end(), values.begin(), values.end());

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_list_commands::optional_string_awaitable_type celeritas::redis_list_commands::async_left_pop(const std::string& key) const
{
    co_return co_await async_execute_command_return_optional_string({ "LPOP", get_prefixed_key(key) });
}

celeritas::redis_list_commands::optional_string_awaitable_type celeritas::redis_list_commands::async_right_pop(const std::string& key) const
{
    co_return co_await async_execute_command_return_optional_string({ "RPOP", get_prefixed_key(key) });
}

celeritas::redis_list_commands::int_awaitable_type celeritas::redis_list_commands::async_get_length(const std::string& key) const
{
    co_return co_await async_execute_command_return_int({ "LLEN", get_prefixed_key(key) });
}

celeritas::redis_list_commands::array_awaitable_type celeritas::redis_list_commands::async_get_range(const std::string& key, const int start, const int end) const
{
    co_return co_await async_execute_command_return_array_type({ "LRANGE", get_prefixed_key(key), std::to_string(start), std::to_string(end) });
}

celeritas::redis_list_commands::int_awaitable_type celeritas::redis_list_commands::async_remove(const std::string& key, const int count, const std::string& value) const
{
    co_return co_await async_execute_command_return_int({ "LREM", get_prefixed_key(key), std::to_string(count), value });
}

celeritas::redis_list_commands::blocking_left_pop_awaitable_type celeritas::redis_list_commands::async_blocking_left_pop(const key_container& keys, const int timeout_seconds) const
{
    if (keys.empty())
    {
        throw celeritas_error{ "blocking left pop requires at least one key." };
    }

    array_type command{ "BLPOP" };
    command.insert(command.end(), keys.begin(), keys.end());
    command.emplace_back(std::to_string(timeout_seconds));

    const auto array_result = co_await async_execute_command_return_array_type(command);

    if (array_result.empty())
    {
        co_return std::nullopt;
    }

    if (array_result.size() != 2)
    {
        throw celeritas_error{ "blocking left pop  returned an array with an unexpected number of elements." };
    }

    co_return blocking_left_pop_result_type{ { array_result[0], array_result[1] } };
}