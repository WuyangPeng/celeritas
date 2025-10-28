#include "server_config.h"

celeritas::server_config::server_config(std::string instance_id, std::string service_name, server_network_config_container_type server_network_config, std::string game_server_id, std::string host, const int worker_pool_size) noexcept
    : instance_id_{ std::move(instance_id) }, service_name_{ std::move(service_name) }, server_network_config_{ std::move(server_network_config) }, game_server_id_{ std::move(game_server_id) }, host_{ std::move(host) }, worker_pool_size_{ worker_pool_size }
{
}

celeritas::server_config::server_network_config_const_iterator celeritas::server_config::begin() const noexcept
{
    return server_network_config_.begin();
}

celeritas::server_config::server_network_config_const_iterator celeritas::server_config::end() const noexcept
{
    return server_network_config_.end();
}

std::string celeritas::server_config::get_instance_id() const
{
    return instance_id_;
}

std::string celeritas::server_config::get_game_server_id() const
{
    return game_server_id_;
}

std::string celeritas::server_config::get_service_name() const
{
    return service_name_;
}

bool celeritas::server_config::is_service_registry_server() const
{
    return service_name_.find("service_registry") != std::string::npos;
}

int celeritas::server_config::get_worker_pool_size() const noexcept
{
    return worker_pool_size_;
}

std::string celeritas::server_config::get_host() const
{
    return host_;
}