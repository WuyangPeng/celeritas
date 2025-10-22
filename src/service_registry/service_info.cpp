#include "service_info.h"

celeritas::service_info::service_info(std::string instance_id,
                                      std::string service_name,
                                      std::string host,
                                      std::string game_server_id,
                                      const int tcp_port,
                                      const int http_port,
                                      const int websocket_port,
                                      const int tcp_ssl_port,
                                      const int https_port,
                                      const int websocket_secure_port)
    : instance_id_{ std::move(instance_id) },
      service_name_{ std::move(service_name) },
      host_{ std::move(host) },
      game_server_id_{ std::move(game_server_id) },
      last_heartbeat_{ std::chrono::steady_clock::now() },
      tcp_port_{ tcp_port },
      http_port_{ http_port },
      websocket_port_{ websocket_port },
      tcp_ssl_port_{ tcp_ssl_port },
      https_port_{ https_port },
      websocket_secure_port_{ websocket_secure_port }
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

int celeritas::service_info::get_tcp_port() const
{
    return tcp_port_;
}

int celeritas::service_info::get_tcp_ssl_port() const
{
    return tcp_ssl_port_;
}

int celeritas::service_info::get_http_port() const
{
    return http_port_;
}

int celeritas::service_info::get_https_port() const
{
    return https_port_;
}

int celeritas::service_info::get_websocket_port() const
{
    return websocket_port_;
}

int celeritas::service_info::get_websocket_secure_port() const
{
    return websocket_secure_port_;
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