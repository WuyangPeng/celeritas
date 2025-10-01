#pragma once

#include "server_network_config.h"

#include <string>
#include <vector>

namespace celeritas
{
    class server_config
    {
    public:
        using class_type = server_config;
        using server_network_config_container_type = std::vector<server_network_config>;

        server_config() noexcept = default;

        server_config(std::string service_name, server_network_config_container_type server_network_config, std::string game_server_id) noexcept;

    private:
        std::string service_name_;
        server_network_config_container_type server_network_config_;
        std::string game_server_id_;
    };
}
