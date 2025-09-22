#include "service_info.h"

celeritas::service_info::service_info(std::string instance_id, std::string service_name, std::string host, const int port, std::string game_server_id)
    : instance_id{ std::move(instance_id) }, service_name{ std::move(service_name) }, host{ std::move(host) }, port{ port }, game_server_id{ std::move(game_server_id) }, last_heartbeat{ std::chrono::steady_clock::now() }
{
}

std::string celeritas::service_info::get_instance_id() const
{
    return instance_id;
}

std::string celeritas::service_info::get_service_name() const
{
    return service_name;
}

std::string celeritas::service_info::get_host() const
{
    return host;
}

int celeritas::service_info::get_port() const
{
    return port;
}

std::string celeritas::service_info::get_game_server_id() const
{
    return game_server_id;
}

celeritas::service_info::time_point_type celeritas::service_info::get_last_heartbeat() const
{
    return last_heartbeat;
}

void celeritas::service_info::set_last_heartbeat()
{
    last_heartbeat = std::chrono::steady_clock::now();
}