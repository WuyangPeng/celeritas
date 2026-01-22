#include "redis_set_commands.h"
#include "common/core/celeritas_error.h"

celeritas::redis_set_commands::redis_set_commands(redis_database_session& session) noexcept
    : base_type{ session }
{
}

celeritas::redis_set_commands::int_awaitable_type celeritas::redis_set_commands::async_add(const std::string& key, const std::string& member) const
{
    co_return co_await async_execute_command_return_int({ "SADD", get_prefixed_key(key), member });
}

celeritas::redis_set_commands::int_awaitable_type celeritas::redis_set_commands::async_add_many(const std::string& key, const array_type& members) const
{
    array_type command{ "SADD", get_prefixed_key(key) };
    command.insert(command.end(), members.begin(), members.end());

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_set_commands::int_awaitable_type celeritas::redis_set_commands::async_remove(const std::string& key, const std::string& member) const
{
    co_return co_await async_execute_command_return_int({ "SREM", get_prefixed_key(key), member });
}

celeritas::redis_set_commands::int_awaitable_type celeritas::redis_set_commands::async_remove_many(const std::string& key, const array_type& members) const
{
    array_type command{ "SREM", get_prefixed_key(key) };
    command.insert(command.end(), members.begin(), members.end());

    co_return co_await async_execute_command_return_int(command);
}

celeritas::redis_set_commands::int_awaitable_type celeritas::redis_set_commands::async_set_cardinality(const std::string& key) const
{
    co_return co_await async_execute_command_return_int({ "SCARD", get_prefixed_key(key) });
}

celeritas::redis_set_commands::bool_awaitable_type celeritas::redis_set_commands::async_set_is_member(const std::string& key, const std::string& member) const
{
    const auto result = co_await async_execute_command_return_int({ "SISMEMBER", get_prefixed_key(key), member });

    co_return result == 1;
}

celeritas::redis_set_commands::array_awaitable_type celeritas::redis_set_commands::async_set_members(const std::string& key) const
{
    co_return co_await async_execute_command_return_array({ "SMEMBERS", get_prefixed_key(key) });
}

celeritas::redis_set_commands::array_awaitable_type celeritas::redis_set_commands::async_set_union(const array_type& keys) const
{
    array_type command{ "SUNION" };
    for (const auto& key : keys)
    {
        command.emplace_back(get_prefixed_key(key));
    }

    co_return co_await async_execute_command_return_array(command);
}

celeritas::redis_set_commands::array_awaitable_type celeritas::redis_set_commands::async_set_inter(const array_type& keys) const
{
    array_type command{ "SINTER" };
    for (const auto& key : keys)
    {
        command.emplace_back(get_prefixed_key(key));
    }

    co_return co_await async_execute_command_return_array(command);
}

celeritas::redis_set_commands::array_awaitable_type celeritas::redis_set_commands::async_set_diff(const array_type& keys) const
{
    array_type command{ "SDIFF" };
    for (const auto& key : keys)
    {
        command.emplace_back(get_prefixed_key(key));
    }

    co_return co_await async_execute_command_return_array(command);
}