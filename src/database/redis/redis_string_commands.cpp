#include "redis_string_commands.h"
#include "common/core/celeritas_error.h"

celeritas::redis_string_commands::redis_string_commands(redis_database_session& session) noexcept
    : base_type{ session }
{
}

celeritas::redis_string_commands::bool_awaitable_type celeritas::redis_string_commands::async_set(const std::string& key, const std::string& value, const int expire_seconds) const
{
    array_type set_command{ "SET", get_prefixed_key(key), value };
    const auto expire_seconds_command = get_expire_seconds_command(expire_seconds);
    set_command.insert(set_command.end(), expire_seconds_command.begin(), expire_seconds_command.end());

    co_return co_await async_execute_command_is_ok(set_command);
}

celeritas::redis_string_commands::bool_awaitable_type celeritas::redis_string_commands::async_set_not_exists(const std::string& key, const std::string& value, const int expire_seconds) const
{
    const auto expire_seconds_comma = get_expire_seconds_command(expire_seconds);

    if (expire_seconds_comma.empty())
    {
        throw celeritas_error{ "the expiration time is invalid." };
    }

    array_type set_command{ "SET", get_prefixed_key(key), value };
    set_command.insert(set_command.end(), expire_seconds_comma.begin(), expire_seconds_comma.end());
    set_command.emplace_back("NX");

    co_return co_await async_execute_command_is_ok(set_command);
}

celeritas::redis_string_commands::bool_awaitable_type celeritas::redis_string_commands::async_set_exists(const std::string& key, const std::string& value, const int expire_seconds) const
{
    const auto expire_seconds_comma = get_expire_seconds_command(expire_seconds);

    array_type set_command{ "SET", get_prefixed_key(key), value };
    set_command.insert(set_command.end(), expire_seconds_comma.begin(), expire_seconds_comma.end());
    set_command.emplace_back("XX");

    co_return co_await async_execute_command_is_ok(set_command);
}

celeritas::redis_string_commands::bool_awaitable_type celeritas::redis_string_commands::async_set_many(const key_value_container& key_values) const
{
    if (key_values.empty())
    {
        co_return false;
    }

    array_type set_command{ "MSET" };
    const auto keys_value_command = get_keys_value_command(key_values);
    set_command.insert(set_command.end(), keys_value_command.begin(), keys_value_command.end());

    co_return co_await async_execute_command_is_ok(set_command);
}

celeritas::redis_string_commands::int_awaitable_type celeritas::redis_string_commands::async_append(const std::string& key, const std::string& value) const
{
    co_return co_await async_execute_command_return_int({ "APPEND", get_prefixed_key(key), value });
}

celeritas::redis_string_commands::int_awaitable_type celeritas::redis_string_commands::async_increment_by(const std::string& key, const int increment) const
{
    co_return co_await async_execute_command_return_int({ "INCRBY", get_prefixed_key(key), std::to_string(increment) });
}

celeritas::redis_string_commands::int_awaitable_type celeritas::redis_string_commands::async_increment(const std::string& key) const
{
    co_return co_await async_execute_command_return_int({ "INCR", get_prefixed_key(key) });
}

celeritas::redis_string_commands::int_awaitable_type celeritas::redis_string_commands::async_decrement_by(const std::string& key, const int decrement) const
{
    co_return co_await async_execute_command_return_int({ "DECRBY", get_prefixed_key(key), std::to_string(decrement) });
}

celeritas::redis_string_commands::int_awaitable_type celeritas::redis_string_commands::async_decrement(const std::string& key) const
{
    co_return co_await async_execute_command_return_int({ "DECR", get_prefixed_key(key) });
}

celeritas::redis_string_commands::optional_string_awaitable_type celeritas::redis_string_commands::async_get(const std::string& key) const
{
    co_return co_await async_execute_command_return_optional_string({ "GET", get_prefixed_key(key) });
}

celeritas::redis_string_commands::array_awaitable_type celeritas::redis_string_commands::async_get_many(const key_container& keys) const
{
    if (keys.empty())
    {
        co_return array_type{};
    }

    array_type get_many_command{ "MGET" };
    const auto keys_command = get_keys_command(keys);
    get_many_command.insert(get_many_command.end(), keys_command.begin(), keys_command.end());

    co_return co_await async_execute_command_return_array(get_many_command);
}

celeritas::redis_string_commands::optional_string_awaitable_type celeritas::redis_string_commands::async_get_set(const std::string& key, const std::string& value) const
{
    co_return co_await async_execute_command_return_optional_string({ "GETSET", get_prefixed_key(key), value });
}
