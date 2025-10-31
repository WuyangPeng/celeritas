#include "service_info.h"

celeritas::service_info::service_info(std::string instance_id,
                                      std::string service_name,
                                      std::string host,
                                      std::string game_server_id,
                                      protocol_port_container protocol_port)
    : instance_id_{ std::move(instance_id) },
      service_name_{ std::move(service_name) },
      host_{ std::move(host) },
      game_server_id_{ std::move(game_server_id) },
      last_heartbeat_{ std::chrono::steady_clock::now() },
      protocol_port_{ std::move(protocol_port) },
      is_heartbeat_{ true }
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

void celeritas::service_info::set_last_heartbeat()
{
    last_heartbeat_ = std::chrono::steady_clock::now();
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

bool celeritas::service_info::is_heartbeat() const noexcept
{
    return is_heartbeat_;
}

void celeritas::service_info::set_heartbeat(bool is_heartbeat)
{
    is_heartbeat_ = is_heartbeat;
}