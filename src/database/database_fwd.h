#pragma once

#include <chrono>

namespace celeritas
{
    template <typename SessionType>
    class connection_pool_base;

    class database_pool_manager;
    class database_pool;
    class database_session;
    class mysql_database_session;
    class mongo_database_session;
    class redis_database_session;
    class redis_commands;
    class redis_key_commands;
    class redis_string_commands;
    class redis_hash_commands;
    class redis_list_commands;

    constexpr auto cleanup_database_timer = std::chrono::hours(1);
    constexpr auto cleanup_database_expired_timer = std::chrono::hours(1);
}
