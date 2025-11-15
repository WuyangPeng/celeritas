#include "redis_database_session.h"
#include "redis_key_commands.h"
#include "common/celeritas_error.h"

celeritas::redis_key_commands::redis_key_commands(redis_database_session& session) noexcept
    : base_type{ session }
{
}

celeritas::redis_key_commands::int_awaitable_type celeritas::redis_key_commands::async_delete(const std::string& key) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto del_command = "DEL " + prefixed_key;

    co_return co_await async_execute_command_return_int(del_command);
}

celeritas::redis_key_commands::int_awaitable_type celeritas::redis_key_commands::async_delete_many(const key_container& keys) const
{
    if (keys.empty())
    {
        co_return 0;
    }

    const auto del_command = "DEL" + get_keys_command(keys);

    co_return co_await async_execute_command_return_int(del_command);
}

celeritas::redis_key_commands::bool_awaitable_type celeritas::redis_key_commands::set_expire_seconds(const std::string& key, const int expire_seconds) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto expire_command = "EXPIRE " + prefixed_key + " " + std::to_string(expire_seconds);

    const auto result = co_await async_execute_command_return_int(expire_command);

    co_return result > 0;
}

celeritas::redis_key_commands::int_awaitable_type celeritas::redis_key_commands::get_expire_seconds(const std::string& key) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto expire_command = "TTL " + prefixed_key;

    co_return co_await async_execute_command_return_int(expire_command);
}

celeritas::redis_key_commands::bool_awaitable_type celeritas::redis_key_commands::is_exists(const std::string& key) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto exists_command = "EXISTS " + prefixed_key;

    const auto result = co_await async_execute_command_return_int(exists_command);

    co_return result > 0;
}

celeritas::redis_key_commands::int_awaitable_type celeritas::redis_key_commands::is_exists_many(const key_container& keys) const
{
    if (keys.empty())
    {
        co_return 0;
    }

    const auto exists_command = "EXISTS" + get_keys_command(keys);

    co_return co_await async_execute_command_return_int(exists_command);
}

celeritas::redis_key_commands::bool_awaitable_type celeritas::redis_key_commands::rename(const std::string& old_key, const std::string& new_key) const
{
    const auto old_prefixed_key = get_prefixed_key(old_key);
    const auto new_prefixed_key = get_prefixed_key(new_key);

    const auto rename_command = "RENAME " + old_prefixed_key + " " + new_prefixed_key;
    if (const auto result = co_await async_execute_command_return_optional_string(rename_command);
        result && *result == "OK")
    {
        co_return true;
    }

    co_return false;
}

celeritas::redis_key_commands::string_awaitable_type celeritas::redis_key_commands::get_type(const std::string& key) const
{
    const auto prefixed_key = get_prefixed_key(key);

    const auto type_command = "TYPE " + prefixed_key;

    if (const auto result = co_await async_execute_command_return_optional_string(type_command);
        result)
    {
        co_return *result;
    }

    throw celeritas_error("get type is error.");
}


