#pragma once

#include <string_view>

namespace celeritas
{
    enum class database_data_type;
    enum class database_index_type;
    enum class database_change_type;

    template <database_data_type Type>
    struct database_data_type_traits;

    class database_field;
    class basis_database;
    class basis_database_container;
    class database_entity_change;
    class database_select_options;
    enum class sort_order;

    template <const std::string_view& FieldName, database_data_type Type, database_index_type Index>
    class entity;

    class database_entity;

    class database_session;
    class mysql_database_session;
    class mongo_database_session;
    class redis_database_session;

    template <typename SessionType>
    class connection_pool;

    template <typename SessionType>
    class database_session_guard;

    class database_pool_base;
    class database_pool_manager;

    class redis_commands;
    class redis_key_commands;
    class redis_string_commands;
    class redis_hash_commands;
    class redis_list_commands;
    class redis_set_commands;
    class redis_sorted_set_commands;
    class sorted_set_member_score;
    class scan_result;

    class config_manager;
    class config_table_base;

    template <typename T>
    class config_table;
}
