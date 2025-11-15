#include "redis_database_session.h"
#include "redis_set_commands.h"
#include "common/celeritas_error.h"

using namespace celeritas;

redis_set_commands::redis_set_commands(redis_database_session& session) noexcept
    : base_type{ session }
{
}

redis_set_commands::int_awaitable_type redis_set_commands::async_add(const std::string& key, const std::string& member) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = "SADD " + prefixed_key + " \"" + member + "\"";

    co_return co_await async_execute_command_return_int(command);
}

redis_set_commands::int_awaitable_type redis_set_commands::async_add_many(const std::string& key, const array_type& members) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = "SADD " + prefixed_key + get_fields_command(members);

    co_return co_await async_execute_command_return_int(command);
}

redis_set_commands::int_awaitable_type redis_set_commands::async_remove(const std::string& key, const std::string& member) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = "SREM " + prefixed_key + " \"" + member + "\"";

    co_return co_await async_execute_command_return_int(command);
}

redis_set_commands::int_awaitable_type redis_set_commands::async_remove_many(const std::string& key, const array_type& members) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = "SREM " + prefixed_key + get_fields_command(members);

    co_return co_await async_execute_command_return_int(command);
}

redis_set_commands::int_awaitable_type redis_set_commands::async_set_cardinality(const std::string& key) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = "SCARD " + prefixed_key;

    co_return co_await async_execute_command_return_int(command);
}

redis_set_commands::bool_awaitable_type redis_set_commands::async_set_is_member(const std::string& key, const std::string& member) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = "SISMEMBER " + prefixed_key + " \"" + member + "\"";

    const auto result = co_await async_execute_command_return_int(command);

    co_return result == 1;
}

redis_set_commands::array_awaitable_type redis_set_commands::async_set_members(const std::string& key) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto command = std::string("SMEMBERS ") + prefixed_key;

    co_return co_await async_execute_command_return_array_type(command);
}

redis_set_commands::array_awaitable_type redis_set_commands::async_set_union(const array_type& keys) const
{
    const auto command = "SUNION " + get_keys_command(keys);

    co_return co_await async_execute_command_return_array_type(command);
}

redis_set_commands::array_awaitable_type redis_set_commands::async_set_inter(const array_type& keys) const
{
    const auto command = "SINTER " + get_keys_command(keys);

    co_return co_await async_execute_command_return_array_type(command);
}

redis_set_commands::array_awaitable_type redis_set_commands::async_set_diff(const array_type& keys) const
{
    const auto command = "SDIFF " + get_keys_command(keys);

    co_return co_await async_execute_command_return_array_type(command);
}