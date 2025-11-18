#include "redis_commands.h"
#include "redis_database_session.h"

celeritas::redis_commands::redis_commands(redis_database_session& session) noexcept
    : session_{ session }
{
}

std::string celeritas::redis_commands::get_keys_command(const key_container& keys) const
{
    std::string command{};
    for (const auto& key : keys)
    {
        command += " " + session_.get_prefixed_key(key);
    }

    return command;
}

std::string celeritas::redis_commands::get_fields_command(const key_container& fields) const
{
    std::string command{};
    for (const auto& field : fields)
    {
        command += " " + field;
    }

    return command;
}

std::string celeritas::redis_commands::get_values_command(const key_container& values) const
{
    std::string command{};
    for (const auto& value : values)
    {
        command += " \"" + value + "\"";
    }

    return command;
}

std::string celeritas::redis_commands::get_keys_value_command(const key_value_container& key_values) const
{
    std::string command{};
    for (const auto& [key, value] : key_values)
    {
        const auto prefixed_key = session_.get_prefixed_key(key);

        command += " " + prefixed_key + " \"" + value + "\"";
    }

    return command;
}

std::string celeritas::redis_commands::get_fields_value_command(const key_value_container& field_values) const
{
    std::string command{};
    for (const auto& [field, value] : field_values)
    {
        command += " " + field + " \"" + value + "\"";
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

std::string celeritas::redis_commands::get_expire_seconds_command(int expire_seconds) const
{
    return session_.get_expire_seconds_command(expire_seconds);
}

celeritas::redis_commands::int_awaitable_type celeritas::redis_commands::async_execute_command_return_int(const std::string& command) const
{
    co_return co_await session_.async_execute_command_return_int(command);
}

celeritas::redis_commands::void_awaitable_type celeritas::redis_commands::async_execute_command_return_void(const std::string& command) const
{
    co_return co_await session_.async_execute_command_return_void(command);
}

celeritas::redis_commands::optional_string_awaitable_type celeritas::redis_commands::async_execute_command_return_optional_string(const std::string& command) const
{
    co_return co_await session_.async_execute_command_return_optional_string(command);
}

celeritas::redis_commands::array_awaitable_type celeritas::redis_commands::async_execute_command_return_array_type(const std::string& command) const
{
    co_return co_await session_.async_execute_command_return_array_type(command);
}

celeritas::redis_commands::map_awaitable_type celeritas::redis_commands::async_execute_command_return_map_type(const std::string& command) const
{
    co_return co_await session_.async_execute_command_return_map_type(command);
}

celeritas::redis_commands::optional_double_awaitable_type celeritas::redis_commands::async_execute_command_return_optional_double(const std::string& command) const
{
    co_return co_await session_.async_execute_command_return_optional_double(command);
}

celeritas::redis_commands::optional_int_awaitable_type celeritas::redis_commands::async_execute_command_return_optional_int(const std::string& command) const
{
    co_return co_await session_.async_execute_command_return_optional_int(command);
}

celeritas::redis_commands::scan_result_awaitable_type celeritas::redis_commands::async_execute_command_return_scan_result(const std::string& command) const
{
    co_return co_await session_.async_execute_command_return_scan_result(command);
}

celeritas::redis_commands::bool_awaitable_type celeritas::redis_commands::async_execute_command_is_ok(const std::string& command) const
{
    if (const auto result = co_await async_execute_command_return_optional_string(command);
        result && *result == "OK")
    {
        co_return true;
    }

    co_return false;
}

std::string celeritas::redis_commands::get_quoted_value_command(const std::string& value) const
{
    return "\"" + value + "\"";
}