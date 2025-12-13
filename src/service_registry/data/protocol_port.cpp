#include "protocol_port.h"

celeritas::protocol_port::protocol_port(const server_network_type server_network_type, const int port)
    : server_network_type_{ server_network_type }, port_{ port }
{
}

celeritas::server_network_type celeritas::protocol_port::get_server_network_type() const noexcept
{
    return server_network_type_;
}

int celeritas::protocol_port::get_port() const noexcept
{
    return port_;
}