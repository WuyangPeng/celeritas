#include "redis_database_session.h"
#include "redis_list_commands.h"
#include "common/celeritas_error.h"

celeritas::redis_list_commands::redis_list_commands(redis_database_session& session) noexcept
    : base_type{ session }
{
}

celeritas::redis_list_commands::int_awaitable_type celeritas::redis_list_commands::async_left_push(const std::string& key, const std::string& value) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = "LPUSH " + prefixed_key + " \"" + value + "\"";

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_list_commands::int_awaitable_type celeritas::redis_list_commands::async_left_push_many(const std::string& key, const key_container& values) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = "LPUSH " + prefixed_key + get_fields_command(values);

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_list_commands::int_awaitable_type celeritas::redis_list_commands::async_right_push(const std::string& key, const std::string& value) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = "RPUSH " + prefixed_key + " \"" + value + "\"";

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_list_commands::int_awaitable_type celeritas::redis_list_commands::async_right_push_many(const std::string& key, const key_container& values) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = "RPUSH " + prefixed_key + get_fields_command(values);

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_list_commands::optional_string_awaitable_type celeritas::redis_list_commands::async_left_pop(const std::string& key) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = std::string("LPOP ") + prefixed_key;

    co_return co_await async_execute_command_return_optional_string(command);
}

celeritas::redis_list_commands::optional_string_awaitable_type celeritas::redis_list_commands::async_right_pop(const std::string& key) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = std::string("RPOP ") + prefixed_key;

    co_return co_await async_execute_command_return_optional_string(command);
}

celeritas::redis_list_commands::int_awaitable_type celeritas::redis_list_commands::async_get_length(const std::string& key) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = std::string("LLEN ") + prefixed_key;

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_list_commands::array_awaitable_type celeritas::redis_list_commands::async_get_range(const std::string& key, const int start, const int end) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = std::string("LRANGE ") + prefixed_key + " " + std::to_string(start) + " " + std::to_string(end);

    co_return co_await async_execute_command_return_array_type(command);
}

celeritas::redis_list_commands::int_awaitable_type celeritas::redis_list_commands::async_remove(const std::string& key, const int count, const std::string& value) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = std::string("LREM ") + prefixed_key + " " + std::to_string(count) + " \"" + value + "\"";

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_list_commands::blocking_left_pop_awaitable_type celeritas::redis_list_commands::async_blocking_left_pop(const key_container& keys, int timeout_seconds) const
{
    if (keys.empty())
    {
        throw celeritas_error("blocking left pop requires at least one key.");
    }

    const auto command = "BLPOP" + get_keys_command(keys) + " " + std::to_string(timeout_seconds);

    const auto array_result = co_await async_execute_command_return_array_type(command);

    if (array_result.empty())
    {
        co_return std::nullopt;
    }

    if (array_result.size() != 2)
    {
        throw celeritas_error("blocking left pop  returned an array with an unexpected number of elements.");
    }

    co_return blocking_left_pop_result_type{ std::make_pair(std::move(array_result[0]), std::move(array_result[1])) };
}