#include "test_redis_commands.h"

celeritas::test_redis_commands::test_redis_commands(redis_database_session& session) noexcept
    : redis_commands{ session }
{
}

celeritas::redis_commands::array_type celeritas::test_redis_commands::test_get_keys_command(const key_container& keys) const
{
    return get_keys_command(keys);
}

celeritas::redis_commands::array_type celeritas::test_redis_commands::test_get_keys_value_command(const key_value_container& key_values) const
{
    return get_keys_value_command(key_values);
}

celeritas::redis_commands::array_type celeritas::test_redis_commands::test_get_fields_value_command(const key_value_container& field_values)
{
    return get_fields_value_command(field_values);
}

std::string celeritas::test_redis_commands::test_get_prefixed_key(const std::string& key) const
{
    return get_prefixed_key(key);
}

celeritas::redis_commands::array_type celeritas::test_redis_commands::test_get_expire_seconds_command(int expire_seconds) const
{
    return get_expire_seconds_command(expire_seconds);
}