#pragma once

namespace celeritas
{
    enum class server_network_type;
    enum class database_type;

    class server_network_config;
    class logger_level_config;
    class service_registry_config;
    class health_check_url_config;
    class database_config;
    class logger_config;
    class global_config;

    class server_config;
    class app_config;

    class game_config;
    class game_tables;
    class pretreatment_config;

    template <typename Element, typename IndexType>
    class container_config;

    class weight;
    class weight_config;
    class rename_cost_config;
}