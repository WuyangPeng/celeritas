#include "server_config.h"
#include "initializer/initializer_constant.h"

celeritas::server_config::server_config(std::string instance_id,
                                        std::string service_name,
                                        server_network_config_container_type server_network_config,
                                        std::string game_server_id,
                                        std::string host,
                                        const int worker_pool_size,
                                        const int datacenter_id,
                                        const int worker_id,
                                        const bool load_game_config) noexcept
    : instance_id_{ std::move(instance_id) },
      service_name_{ std::move(service_name) },
      server_network_config_{ std::move(server_network_config) },
      game_server_id_{ std::move(game_server_id) },
      host_{ std::move(host) },
      worker_pool_size_{ worker_pool_size },
      datacenter_id_{ datacenter_id },
      worker_id_{ worker_id },
      load_game_config_{ load_game_config }
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
    return service_name_.find(service_registry_type) != std::string::npos;
}

int celeritas::server_config::get_worker_pool_size() const noexcept
{
    return worker_pool_size_;
}

std::string celeritas::server_config::get_host() const
{
    return host_;
}

int celeritas::server_config::get_datacenter_id() const noexcept
{
    return datacenter_id_;
}

int celeritas::server_config::get_worker_id() const noexcept
{
    return worker_id_;
}

bool celeritas::server_config::is_load_game_config() const noexcept
{
    return load_game_config_;
}