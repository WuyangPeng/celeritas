#include "server_config_fixture.h"

const std::string celeritas::server_config_fixture::instance_id{ "server_01" };
const std::string celeritas::server_config_fixture::service_name{ "game_service" };
const std::string celeritas::server_config_fixture::game_server_id{ "game_server_id_01" };
const std::string celeritas::server_config_fixture::host{ "127.0.0.1" };

celeritas::server_config celeritas::server_config_fixture::create_config(const server_network_container& networks)
{
    return { instance_id, service_name, networks, game_server_id, host, worker_pool_size, datacenter_id, worker_id, load_game_config };
}

celeritas::server_config celeritas::server_config_fixture::create_config()
{
    return create_config({});
}