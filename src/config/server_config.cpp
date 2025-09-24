#include "server_config.h"

celeritas::server_config::server_config(std::string service_name, std::string host, int port, std::string game_server_id) noexcept
    : service_name_{ std::move(service_name) }, host_{ host }, port_{ port }, game_server_id_{ game_server_id }
{
}