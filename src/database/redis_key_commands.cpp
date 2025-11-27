#include "redis_key_commands.h"
#include "common/celeritas_error.h"

celeritas::redis_key_commands::redis_key_commands(redis_database_session& session) noexcept
    : base_type{ session }
{
}

celeritas::redis_key_commands::int_awaitable_type celeritas::redis_key_commands::async_delete(const std::string& key) const
{
    const auto del_command = "DEL " + get_prefixed_key(key);

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

celeritas::redis_key_commands::bool_awaitable_type celeritas::redis_key_commands::async_set_expire_seconds(const std::string& key, const int expire_seconds) const
{
    const auto expire_command = "EXPIRE " + get_prefixed_key(key) + " " + std::to_string(expire_seconds);

    const auto result = co_await async_execute_command_return_int(expire_command);

    co_return result > 0;
}

celeritas::redis_key_commands::int_awaitable_type celeritas::redis_key_commands::async_get_expire_seconds(const std::string& key) const
{
    const auto expire_command = "TTL " + get_prefixed_key(key);

    co_return co_await async_execute_command_return_int(expire_command);
}

celeritas::redis_key_commands::bool_awaitable_type celeritas::redis_key_commands::async_is_exists(const std::string& key) const
{
    const auto exists_command = "EXISTS " + get_prefixed_key(key);

    const auto result = co_await async_execute_command_return_int(exists_command);

    co_return result > 0;
}

celeritas::redis_key_commands::int_awaitable_type celeritas::redis_key_commands::async_is_exists_many(const key_container& keys) const
{
    if (keys.empty())
    {
        co_return 0;
    }

    const auto exists_command = "EXISTS" + get_keys_command(keys);

    co_return co_await async_execute_command_return_int(exists_command);
}

celeritas::redis_key_commands::bool_awaitable_type celeritas::redis_key_commands::async_rename(const std::string& old_key, const std::string& new_key) const
{
    const auto rename_command = "RENAME " + get_prefixed_key(old_key) + " " + get_prefixed_key(new_key);

    co_return co_await async_execute_command_is_ok(rename_command);
}

celeritas::redis_key_commands::string_awaitable_type celeritas::redis_key_commands::async_get_type(const std::string& key) const
{
    const auto type_command = "TYPE " + get_prefixed_key(key);

    if (const auto result = co_await async_execute_command_return_optional_string(type_command);
        result)
    {
        co_return *result;
    }

    throw celeritas_error{ "get type is error." };
}

celeritas::redis_commands::scan_result_awaitable_type celeritas::redis_key_commands::async_scan(const std::string& pattern, const int cursor, const int count) const
{
    const auto scan_command = "SCAN " +
                              std::to_string(cursor) +
                              " MATCH " +
                              get_prefixed_key(pattern) +
                              " COUNT " +
                              std::to_string(count);

    co_return co_await async_execute_command_return_scan_result(scan_command);
}

celeritas::redis_commands::array_awaitable_type celeritas::redis_key_commands::async_scan_all(const std::string& pattern) const
{
    auto scan = co_await async_scan(pattern, 0, redis_cursor_one_request_size);

    array_type keys{};
    while (scan.get_cursor() != "0")
    {
        scan = co_await async_scan(pattern, std::stoi(scan.get_cursor()), redis_cursor_one_request_size);
        keys.insert(keys.cend(), scan.get_keys().cbegin(), scan.get_keys().cend());
    }

    co_return keys;
}


