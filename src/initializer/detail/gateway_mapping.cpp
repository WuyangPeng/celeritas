#include "gateway_mapping.h"

celeritas::gateway_mapping::gateway_mapping(const server_network_type server_network_type, const int64_t session_id, std::string instance_id)
    : server_network_type_{ server_network_type }, session_id_{ session_id }, instance_id_{ std::move(instance_id) }
{
}

celeritas::server_network_type celeritas::gateway_mapping::get_server_network_type() const
{
    return server_network_type_;
}

int64_t celeritas::gateway_mapping::get_session_id() const
{
    return session_id_;
}

std::string celeritas::gateway_mapping::get_instance_id() const
{
    return instance_id_;
}