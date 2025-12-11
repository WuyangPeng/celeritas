#include "server_resource_loader.h"
#include "common/celeritas_error.h"
#include "config/server_network_type.h"
#include "network/http_listener.h"
#include "network/tcp_listener.h"
#include "network/websocket_listener.h"

celeritas::server_resource_loader::listener_shared_ptr celeritas::server_resource_loader::loader_server(io_context_type& io_context, const server_config& server_config, const server_network_config& server_network_config, const network_message_callback_weak_ptr& network_message_callback)
{
    switch (const auto server_network_type = server_network_config.get_server_network_type())
    {
        case server_network_type::tcp:
        {
            return std::make_shared<tcp_listener>(io_context, network_message_callback, server_config.get_game_server_id(), server_network_config.get_port(), server_network_type);
        }

        case server_network_type::http:
        {
            return std::make_shared<http_listener>(io_context, network_message_callback, server_config.get_game_server_id(), server_network_config.get_port(), server_network_type);
        }

        case server_network_type::websocket:
        {
            return std::make_shared<websocket_listener>(io_context, network_message_callback, server_config.get_game_server_id(), server_network_config.get_port(), server_network_type);
        }

        default:
        {
            throw celeritas_error("create server ,type = " + std::to_string(static_cast<int>(server_network_config.get_server_network_type())) + " is  not exist.");
        }
    }
}