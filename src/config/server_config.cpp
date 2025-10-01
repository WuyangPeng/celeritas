#include "server_config.h"

celeritas::server_config::server_config(std::string service_name, server_network_config_container_type server_network_config, std::string game_server_id) noexcept
    : service_name_{ std::move(service_name) }, server_network_config_{ std::move(server_network_config) }, game_server_id_{ std::move(game_server_id) }
{
}