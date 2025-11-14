#include "redis_database_session.h"
#include "redis_key_commands.h"

celeritas::redis_key_commands::redis_key_commands(redis_database_session& session) noexcept
    : session_{ session }
{
}

celeritas::redis_key_commands::int_awaitable_type celeritas::redis_key_commands::async_delete(const std::string& key) const
{
    const auto prefixed_key = session_.get_prefixed_key(key);

    const auto del_command = std::string("DEL ") + prefixed_key;

    co_return co_await session_.async_execute_command_return_int(del_command);
}

celeritas::redis_key_commands::int_awaitable_type celeritas::redis_key_commands::async_delete_many(const container& keys) const
{
    const auto del_command = std::string("DEL") + get_keys_command(keys);

    co_return co_await session_.async_execute_command_return_int(del_command);
}

celeritas::redis_key_commands::bool_awaitable_type celeritas::redis_key_commands::set_expire_seconds(const std::string& key, const int expire_seconds) const
{
    const auto prefixed_key = session_.get_prefixed_key(key);

    const auto expire_command = std::string("EXPIRE ") + prefixed_key + " " + std::to_string(expire_seconds);

    const auto result = co_await session_.async_execute_command_return_int(expire_command);

    co_return result > 0;
}

celeritas::redis_key_commands::int_awaitable_type celeritas::redis_key_commands::get_expire_seconds(const std::string& key) const
{
    const auto prefixed_key = session_.get_prefixed_key(key);

    const auto expire_command = std::string("TTL ") + prefixed_key;

    co_return co_await session_.async_execute_command_return_int(expire_command);
}

celeritas::redis_key_commands::bool_awaitable_type celeritas::redis_key_commands::is_exists(const std::string& key) const
{
    const auto prefixed_key = session_.get_prefixed_key(key);

    const auto exists_command = std::string("EXISTS ") + prefixed_key;

    const auto result = co_await session_.async_execute_command_return_int(exists_command);

    co_return result > 0;
}

celeritas::redis_key_commands::int_awaitable_type celeritas::redis_key_commands::is_exists_many(const container& keys) const
{
    const auto exists_command = std::string("EXISTS") + get_keys_command(keys);

    co_return co_await session_.async_execute_command_return_int(exists_command);
}

celeritas::redis_key_commands::bool_awaitable_type celeritas::redis_key_commands::rename(const std::string& old_key, const std::string& new_key) const
{
    const auto old_prefixed_key = session_.get_prefixed_key(old_key);
    const auto new_prefixed_key = session_.get_prefixed_key(new_key);

    const auto rename_command = std::string("RENAME ") + old_prefixed_key + " " + new_prefixed_key;

    if (const auto result = co_await session_.async_execute_command_return_string(rename_command);
        result == "OK")
    {
        co_return true;
    }

    co_return false;
}

celeritas::redis_key_commands::string_awaitable_type celeritas::redis_key_commands::get_type(const std::string& key) const
{
    const auto prefixed_key = session_.get_prefixed_key(key);

    const auto type_command = std::string("TYPE ") + prefixed_key;

    co_return co_await session_.async_execute_command_return_string(type_command);
}

std::string celeritas::redis_key_commands::get_keys_command(const container& keys) const
{
    std::string command{};
    for (const auto& key : keys)
    {
        command += " " + session_.get_prefixed_key(key);
    }

    return command;
}
