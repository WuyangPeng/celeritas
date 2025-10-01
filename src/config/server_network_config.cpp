#include "server_network_config.h"

celeritas::server_network_config::server_network_config(server_network_type server_network_type, std::string host, int port) noexcept
    : server_network_type_{ server_network_type }, host_{ host }, port_{ port }
{
}

celeritas::server_network_type celeritas::server_network_config::get_server_network_type() const noexcept
{
    return server_network_type_;
}

int celeritas::server_network_config::get_port() const noexcept
{
    return port_;
}