#include "service_info.h"

celeritas::service_info::service_info(std::string instance_id,
                                      std::string service_name,
                                      std::string host,
                                      std::string game_server_id,
                                      protocol_port_container protocol_port,
                                      int64_t start_server_time)
    : instance_id_{ std::move(instance_id) },
      service_name_{ std::move(service_name) },
      host_{ std::move(host) },
      game_server_id_{ std::move(game_server_id) },
      last_heartbeat_{ std::chrono::steady_clock::now() },
      protocol_port_{ std::move(protocol_port) },
      health_check_level_{ health_check_level_type::health },
      start_server_time_{ start_server_time }
{
}

std::string celeritas::service_info::get_instance_id() const
{
    return instance_id_;
}

std::string celeritas::service_info::get_service_name() const
{
    return service_name_;
}

std::string celeritas::service_info::get_host() const
{
    return host_;
}

std::string celeritas::service_info::get_game_server_id() const
{
    return game_server_id_;
}

celeritas::service_info::time_point_type celeritas::service_info::get_last_heartbeat() const
{
    return last_heartbeat_;
}

void celeritas::service_info::set_last_heartbeat(const int64_t start_server_time)
{
    last_heartbeat_ = std::chrono::steady_clock::now();

    if (start_server_time != start_server_time_)
    {
        start_server_time_ = start_server_time;
        health_check_level_ = health_check_level_type::health;
    }
}

int celeritas::service_info::get_port(const server_network_type server_network_type) const
{
    for (const auto& protocol_port : protocol_port_)
    {
        if (protocol_port.get_server_network_type() == server_network_type)
        {
            return protocol_port.get_port();
        }
    }

    return 0;
}

celeritas::health_check_level_type celeritas::service_info::get_health_check_level_type() const noexcept
{
    return health_check_level_;
}

void celeritas::service_info::set_health_check_level_type(health_check_level_type health_check_level)
{
    health_check_level_ = health_check_level;
}

int64_t celeritas::service_info::get_start_server_time() const
{
    return start_server_time_;
}