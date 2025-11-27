#include "redis_string_commands.h"
#include "common/celeritas_error.h"

celeritas::redis_string_commands::redis_string_commands(redis_database_session& session) noexcept
    : base_type{ session }
{
}

celeritas::redis_string_commands::bool_awaitable_type celeritas::redis_string_commands::async_set(const std::string& key, const std::string& value, const int expire_seconds) const
{
    const auto set_command = "SET " + get_prefixed_key(key) + " " + get_quoted_value_command(value) + get_expire_seconds_command(expire_seconds);

    co_return co_await async_execute_command_is_ok(set_command);
}

celeritas::redis_string_commands::bool_awaitable_type celeritas::redis_string_commands::async_set_not_exists(const std::string& key, const std::string& value, const int expire_seconds) const
{
    const auto expire_seconds_comma = get_expire_seconds_command(expire_seconds);

    if (expire_seconds_comma.empty())
    {
        throw celeritas_error{ "the expiration time is invalid." };
    }

    const auto set_command = "SET " + get_prefixed_key(key) + " " + get_quoted_value_command(value) + expire_seconds_comma + " NX";

    co_return co_await async_execute_command_is_ok(set_command);
}

celeritas::redis_string_commands::bool_awaitable_type celeritas::redis_string_commands::async_set_exists(const std::string& key, const std::string& value, const int expire_seconds) const
{
    const auto set_command = "SET " + get_prefixed_key(key) + " " + get_quoted_value_command(value) + get_expire_seconds_command(expire_seconds) + " XX";

    co_return co_await async_execute_command_is_ok(set_command);
}

celeritas::redis_string_commands::bool_awaitable_type celeritas::redis_string_commands::async_set_many(const key_value_container& key_values) const
{
    if (key_values.empty())
    {
        co_return false;
    }

    const auto set_command = "MSET" + get_keys_value_command(key_values);

    co_return co_await async_execute_command_is_ok(set_command);
}

celeritas::redis_string_commands::int_awaitable_type celeritas::redis_string_commands::async_append(const std::string& key, const std::string& value) const
{
    const auto append_command = "APPEND " + get_prefixed_key(key) + " " + get_quoted_value_command(value);

    co_return co_await async_execute_command_return_int(append_command);
}

celeritas::redis_string_commands::int_awaitable_type celeritas::redis_string_commands::async_increment_by(const std::string& key, const int increment) const
{
    const auto increment_by_command = "INCRBY " + get_prefixed_key(key) + " " + std::to_string(increment);

    co_return co_await async_execute_command_return_int(increment_by_command);
}

celeritas::redis_string_commands::int_awaitable_type celeritas::redis_string_commands::async_increment(const std::string& key) const
{
    const auto increment_command = "INCR " + get_prefixed_key(key);

    co_return co_await async_execute_command_return_int(increment_command);
}

celeritas::redis_string_commands::int_awaitable_type celeritas::redis_string_commands::async_decrement_by(const std::string& key, const int decrement) const
{
    const auto decrement_by_command = std::string("DECRBY ") + get_prefixed_key(key) + " " + std::to_string(decrement);

    co_return co_await async_execute_command_return_int(decrement_by_command);
}

celeritas::redis_string_commands::int_awaitable_type celeritas::redis_string_commands::async_decrement(const std::string& key) const
{
    const auto decrement_command = "DECR " + get_prefixed_key(key);

    co_return co_await async_execute_command_return_int(decrement_command);
}

celeritas::redis_string_commands::optional_string_awaitable_type celeritas::redis_string_commands::async_get(const std::string& key) const
{
    const auto get_command = std::string("GET ") + get_prefixed_key(key);

    co_return co_await async_execute_command_return_optional_string(get_command);
}

celeritas::redis_string_commands::array_awaitable_type celeritas::redis_string_commands::async_get_many(const key_container& keys) const
{
    if (keys.empty())
    {
        co_return array_type{};
    }

    const auto get_many_command = std::string("MGET") + get_keys_command(keys);

    co_return co_await async_execute_command_return_array_type(get_many_command);
}

celeritas::redis_string_commands::optional_string_awaitable_type celeritas::redis_string_commands::async_get_set(const std::string& key, const std::string& value) const
{
    const auto get_set_command = std::string("GETSET ") + get_prefixed_key(key) + " " + get_quoted_value_command(value);

    co_return co_await async_execute_command_return_optional_string(get_set_command);
}
