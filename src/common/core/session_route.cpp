#include "session_route.h"

celeritas::session_route::session_route(const server_network_type server_network_type, const int64_t session_id, std::string instance_id) noexcept
    : server_network_type_{ server_network_type }, session_id_{ session_id }, instance_id_{ std::move(instance_id) }
{
}

celeritas::server_network_type celeritas::session_route::get_server_network_type() const noexcept
{
    return server_network_type_;
}

int64_t celeritas::session_route::get_session_id() const noexcept
{
    return session_id_;
}

std::string celeritas::session_route::get_instance_id() const
{
    return instance_id_;
}