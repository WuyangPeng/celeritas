#include "server_resource_loader.h"
#include "common/celeritas_error.h"
#include "network/tcp_client.h"
#include "config/server_network_type.h"

void celeritas::server_resource_loader::loader_server(io_context_type& io_context, const server_config& server_config, const server_network_config& server_network_config, const network_message_callback_shared_ptr& network_message_callback)
{
    switch (server_network_config.get_server_network_type())
    {
        case server_network_type::tcp:
        {
            break;
        }

        case server_network_type::http:
        {
            break;
        }

        case server_network_type::websocket:
        {
            break;
        }
        default:
        {
            throw celeritas_error("create server ,type = " + std::to_string(static_cast<int>(server_network_config.get_server_network_type())) + " is  not exist.");
        }
    }
}