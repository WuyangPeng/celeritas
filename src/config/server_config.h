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
        using server_network_config_const_iterator = server_network_config_container_type::const_iterator;

        server_config() noexcept = default;

        server_config(std::string service_name, server_network_config_container_type server_network_config, std::string game_server_id) noexcept;

        [[nodiscard]] server_network_config_const_iterator begin() const noexcept;

        [[nodiscard]] server_network_config_const_iterator end() const noexcept;

        [[nodiscard]] std::string get_game_server_id() const noexcept;

    private:
        std::string service_name_;
        server_network_config_container_type server_network_config_;
        std::string game_server_id_;
    };
}
