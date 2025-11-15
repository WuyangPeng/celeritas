#include "redis_hash_commands.h"
#include "boost/mysql/any_connection.hpp"

celeritas::redis_hash_commands::redis_hash_commands(redis_database_session& session) noexcept
    : base_type{ session }
{
}

celeritas::redis_hash_commands::int_awaitable_type celeritas::redis_hash_commands::async_set(const std::string& key, const std::string& field, const std::string& value) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = "HSET " + prefixed_key + " " + field + " \"" + value + "\"";

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_hash_commands::bool_awaitable_type celeritas::redis_hash_commands::async_set_many(const std::string& key, const key_value_container& field_values) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = "HSET " + prefixed_key + get_fields_value_command(field_values);

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_hash_commands::int_awaitable_type celeritas::redis_hash_commands::async_delete(const std::string& key, const std::string& field) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = "HDEL " + prefixed_key + " " + field;

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_hash_commands::int_awaitable_type celeritas::redis_hash_commands::async_delete_many(const std::string& key, const field_container& fields) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = "HDEL " + prefixed_key + get_fields_command(fields);

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_hash_commands::int_awaitable_type celeritas::redis_hash_commands::async_increment_by(const std::string& key, const std::string& field, int increment) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = "HINCRBY " + prefixed_key + " " + field + " " + std::to_string(increment);

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_hash_commands::optional_string_awaitable_type celeritas::redis_hash_commands::async_get(const std::string& key, const std::string& field) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = std::string("HGET ") + prefixed_key + " " + field;

    co_return co_await async_execute_command_return_optional_string(command);
}

celeritas::redis_hash_commands::array_awaitable_type celeritas::redis_hash_commands::async_get_many(const std::string& key, const field_container& fields) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = std::string("HMGET ") + prefixed_key + get_fields_command(fields);

    co_return co_await async_execute_command_return_array_type(command);
}

celeritas::redis_hash_commands::array_awaitable_type celeritas::redis_hash_commands::async_get_fields(const std::string& key) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = std::string("HKEYS ") + prefixed_key;

    co_return co_await async_execute_command_return_array_type(command);
}

celeritas::redis_hash_commands::map_awaitable_type celeritas::redis_hash_commands::async_get_all(const std::string& key) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = std::string("HGETALL ") + prefixed_key;

    co_return co_await async_execute_command_return_map_type(command);
}