#include "server_network_config.h"

celeritas::server_network_config::server_network_config(server_network_type server_network_type, std::string host, int port) noexcept
    : server_network_type_{ server_network_type }, host_{ host }, port_{ port }
{
}