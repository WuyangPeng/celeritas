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
    class redis_set_commands;
    class redis_sorted_set_commands;
    class sorted_set_member_score;
    class basis_database;

    enum class database_data_type;
    enum class database_index_type;
    enum class database_change_type;

    template <database_data_type Type>
    struct database_data_Type_traits;

    constexpr auto cleanup_database_timer = std::chrono::minutes(30);
    constexpr auto cleanup_database_expired_timer = std::chrono::hours(1);
}
