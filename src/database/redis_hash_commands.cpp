#include "redis_hash_commands.h"

celeritas::redis_hash_commands::redis_hash_commands(redis_database_session& session) noexcept
    : base_type{ session }
{
}

celeritas::redis_hash_commands::int_awaitable_type celeritas::redis_hash_commands::async_set(const std::string& key, const std::string& field, const std::string& value) const
{
    const auto command = "HSET " + get_prefixed_key(key) + " " + field + " " + get_quoted_value_command(value);

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_hash_commands::bool_awaitable_type celeritas::redis_hash_commands::async_set_many(const std::string& key, const key_value_container& field_values) const
{
    const auto command = "HSET " + get_prefixed_key(key) + get_fields_value_command(field_values);

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_hash_commands::int_awaitable_type celeritas::redis_hash_commands::async_delete(const std::string& key, const std::string& field) const
{
    const auto command = "HDEL " + get_prefixed_key(key) + " " + field;

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_hash_commands::int_awaitable_type celeritas::redis_hash_commands::async_delete_many(const std::string& key, const array_type& fields) const
{
    const auto command = "HDEL " + get_prefixed_key(key) + get_fields_command(fields);

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_hash_commands::int_awaitable_type celeritas::redis_hash_commands::async_increment_by(const std::string& key, const std::string& field, const int increment) const
{
    const auto command = "HINCRBY " + get_prefixed_key(key) + " " + field + " " + std::to_string(increment);

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_hash_commands::optional_string_awaitable_type celeritas::redis_hash_commands::async_get(const std::string& key, const std::string& field) const
{
    const auto command = std::string("HGET ") + get_prefixed_key(key) + " " + field;

    co_return co_await async_execute_command_return_optional_string(command);
}

celeritas::redis_hash_commands::array_awaitable_type celeritas::redis_hash_commands::async_get_many(const std::string& key, const array_type& fields) const
{
    const auto command = std::string("HMGET ") + get_prefixed_key(key) + get_fields_command(fields);

    co_return co_await async_execute_command_return_array_type(command);
}

celeritas::redis_hash_commands::array_awaitable_type celeritas::redis_hash_commands::async_get_fields(const std::string& key) const
{
    const auto command = std::string("HKEYS ") + get_prefixed_key(key);

    co_return co_await async_execute_command_return_array_type(command);
}

celeritas::redis_hash_commands::optional_map_awaitable_type celeritas::redis_hash_commands::async_get_all(const std::string& key) const
{
    const auto command = std::string("HGETALL ") + get_prefixed_key(key);

    co_return co_await async_execute_command_return_optional_map_type(command);
}