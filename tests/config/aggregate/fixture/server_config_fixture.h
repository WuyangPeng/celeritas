#pragma once

#include "config/aggregate/server_config.h"

#include <string>

namespace celeritas
{
    class server_config_fixture
    {
    public:
        using class_type = server_config_fixture;
        using server_network_container = server_config::server_network_container;

        static const std::string instance_id;
        static const std::string service_name;
        static const std::string game_server_id;
        static const std::string host;
        static constexpr auto worker_pool_size = 8;
        static constexpr auto datacenter_id = 1;
        static constexpr auto worker_id = 2;
        static constexpr auto load_game_config = true;

        [[nodiscard]] static server_config create_config(const server_network_container& networks);

        [[nodiscard]] static server_config create_config();
    };
}