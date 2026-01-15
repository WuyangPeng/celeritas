#include "redis_commands.h"
#include "database/database_constant.h"
#include "database/session/redis_database_session.h"

celeritas::redis_commands::redis_commands(redis_database_session& session) noexcept
    : session_{ session }
{
}

celeritas::redis_commands::array_type celeritas::redis_commands::get_keys_command(const key_container& keys) const
{
    array_type command{};
    for (const auto& key : keys)
    {
        command.emplace_back(session_.get_prefixed_key(key));
    }

    return command;
}

celeritas::redis_commands::array_type celeritas::redis_commands::get_keys_value_command(const key_value_container& key_values) const
{
    array_type command{};
    for (const auto& [key, value] : key_values)
    {
        const auto prefixed_key = session_.get_prefixed_key(key);

        command.emplace_back(prefixed_key);
        command.emplace_back(value);
    }

    return command;
}

celeritas::redis_commands::array_type celeritas::redis_commands::get_fields_value_command(const key_value_container& field_values)
{
    array_type command{};
    for (const auto& [field, value] : field_values)
    {
        command.emplace_back(field);
        command.emplace_back(value);
    }

    return command;
}

celeritas::redis_database_session& celeritas::redis_commands::get_redis_database_session()
{
    return session_;
}

std::string celeritas::redis_commands::get_prefixed_key(const std::string& key) const
{
    return session_.get_prefixed_key(key);
}

celeritas::redis_commands::array_type celeritas::redis_commands::get_expire_seconds_command(const int expire_seconds) const
{
    return session_.get_expire_seconds_command(expire_seconds);
}

celeritas::redis_commands::int_awaitable_type celeritas::redis_commands::async_execute_command_return_int(const array_type& command) const
{
    co_return co_await session_.async_execute_command_return_int(command);
}

celeritas::redis_commands::void_awaitable_type celeritas::redis_commands::async_execute_command_return_void(const array_type& command) const
{
    co_return co_await session_.async_execute_command_return_void(command);
}

celeritas::redis_commands::optional_string_awaitable_type celeritas::redis_commands::async_execute_command_return_optional_string(const array_type& command) const
{
    co_return co_await session_.async_execute_command_return_optional_string(command);
}

celeritas::redis_commands::array_awaitable_type celeritas::redis_commands::async_execute_command_return_array_type(const array_type& command) const
{
    co_return co_await session_.async_execute_command_return_array_type(command);
}

celeritas::redis_commands::map_awaitable_type celeritas::redis_commands::async_execute_command_return_map_type(const array_type& command) const
{
    co_return co_await session_.async_execute_command_return_map_type(command);
}

celeritas::redis_commands::optional_double_awaitable_type celeritas::redis_commands::async_execute_command_return_optional_double(const array_type& command) const
{
    co_return co_await session_.async_execute_command_return_optional_double(command);
}

celeritas::redis_commands::optional_int_awaitable_type celeritas::redis_commands::async_execute_command_return_optional_int(const array_type& command) const
{
    co_return co_await session_.async_execute_command_return_optional_int(command);
}

celeritas::redis_commands::optional_map_awaitable_type celeritas::redis_commands::async_execute_command_return_optional_map_type(const array_type& command) const
{
    co_return co_await session_.async_execute_command_return_optional_map_type(command);
}

celeritas::redis_commands::scan_result_awaitable_type celeritas::redis_commands::async_execute_command_return_scan_result(const array_type& command) const
{
    co_return co_await session_.async_execute_command_return_scan_result(command);
}

celeritas::redis_commands::bool_awaitable_type celeritas::redis_commands::async_execute_command_is_ok(const array_type& command) const
{
    if (const auto result = co_await async_execute_command_return_optional_string(command);
        result && *result == redis_ok)
    {
        co_return true;
    }

    co_return false;
}

celeritas::redis_commands::sorted_set_member_score_awaitable_type celeritas::redis_commands::async_execute_command_return_sorted_set_member_score_type(const array_type& command) const
{
    co_return co_await session_.async_execute_command_return_sorted_set_member_score_type(command);
}