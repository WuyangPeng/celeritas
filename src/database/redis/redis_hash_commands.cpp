#include "redis_hash_commands.h"

celeritas::redis_hash_commands::redis_hash_commands(redis_database_session& session) noexcept
    : base_type{ session }
{
}

celeritas::redis_hash_commands::int_awaitable_type celeritas::redis_hash_commands::async_set(const std::string& key, const std::string& field, const std::string& value) const
{
    co_return co_await async_execute_command_return_int({ "HSET", get_prefixed_key(key), field, value });
}

celeritas::redis_hash_commands::bool_awaitable_type celeritas::redis_hash_commands::async_set_many(const std::string& key, const key_value_container& field_values) const
{
    array_type command{ "HSET", get_prefixed_key(key) };
    const auto fields_value_command = get_fields_value_command(field_values);
    command.insert(command.end(), fields_value_command.begin(), fields_value_command.end());

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_hash_commands::int_awaitable_type celeritas::redis_hash_commands::async_delete(const std::string& key, const std::string& field) const
{
    co_return co_await async_execute_command_return_int({ "HDEL", get_prefixed_key(key), field });
}

celeritas::redis_hash_commands::int_awaitable_type celeritas::redis_hash_commands::async_delete_many(const std::string& key, const array_type& fields) const
{
    array_type command{ "HDEL", get_prefixed_key(key) };
    command.insert(command.end(), fields.begin(), fields.end());

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_hash_commands::int_awaitable_type celeritas::redis_hash_commands::async_increment_by(const std::string& key, const std::string& field, const int increment) const
{
    co_return co_await async_execute_command_return_int({ "HINCRBY", get_prefixed_key(key), field, std::to_string(increment) });
}

celeritas::redis_hash_commands::optional_string_awaitable_type celeritas::redis_hash_commands::async_get(const std::string& key, const std::string& field) const
{
    co_return co_await async_execute_command_return_optional_string({ "HGET", get_prefixed_key(key), field });
}

celeritas::redis_hash_commands::array_awaitable_type celeritas::redis_hash_commands::async_get_many(const std::string& key, const array_type& fields) const
{
    array_type command{ "HMGET", get_prefixed_key(key) };
    command.insert(command.end(), fields.begin(), fields.end());

    co_return co_await async_execute_command_return_array_type(command);
}

celeritas::redis_hash_commands::array_awaitable_type celeritas::redis_hash_commands::async_get_fields(const std::string& key) const
{
    co_return co_await async_execute_command_return_array_type({ "HKEYS", get_prefixed_key(key) });
}

celeritas::redis_hash_commands::optional_map_awaitable_type celeritas::redis_hash_commands::async_get_all(const std::string& key) const
{
    co_return co_await async_execute_command_return_optional_map_type({ "HGETALL", get_prefixed_key(key) });
}

celeritas::redis_commands::optional_map_awaitable_type celeritas::redis_hash_commands::async_get_all_by_real_key(const std::string& key) const
{
    co_return co_await async_execute_command_return_optional_map_type({ "HGETALL", key });
}