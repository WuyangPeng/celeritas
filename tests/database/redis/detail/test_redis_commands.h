#pragma once

#include "database/redis/redis_commands.h"

namespace celeritas
{
    class test_redis_commands final : public redis_commands
    {
    public:
        using class_type = test_redis_commands;
        using base_type = redis_commands;

        explicit test_redis_commands(redis_database_session& session) noexcept;

        [[nodiscard]] array_type test_get_keys_command(const key_container& keys) const;

        [[nodiscard]] array_type test_get_keys_value_command(const key_value_container& key_values) const;

        [[nodiscard]] static array_type test_get_fields_value_command(const key_value_container& field_values);

        [[nodiscard]] std::string test_get_prefixed_key(const std::string& key) const;

        [[nodiscard]] array_type test_get_expire_seconds_command(int expire_seconds) const;

        [[nodiscard]] redis_database_session& test_get_redis_database_session();
    };
}  