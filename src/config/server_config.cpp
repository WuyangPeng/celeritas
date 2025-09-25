#include "server_config.h"

celeritas::server_config::server_config(std::string service_name, std::string host, int tcp_port, std::string game_server_id) noexcept
    : service_name_{ std::move(service_name) }, host_{ std::move(host) }, tcp_port_{ tcp_port }, game_server_id_{ std::move(game_server_id) }
{
}