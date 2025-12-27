#include "listener_accept.h"

celeritas::listener_accept::listener_accept(const server_network_type server_network_type)
    : base_type{ server_network_type }
{
}

bool celeritas::listener_accept::write(const std::string& server_type, const std::string& instance_id, const header& header, const protobuf_message& request)
{
}