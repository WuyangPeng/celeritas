#include "server_network_type.h"
#include "../common/core_utilities/celeritas_error.h"

#include <map>

celeritas::server_network_type celeritas::get_server_network_type(const std::string& server_network_name)
{
    using server_network_container_type = std::map<std::string, server_network_type>;

    static server_network_container_type server_network{ { "tcp", server_network_type::tcp },
                                                         { "http", server_network_type::http },
                                                         { "websocket", server_network_type::websocket },
                                                         { "tcp_ssl", server_network_type::tcp_ssl },
                                                         { "https", server_network_type::https },
                                                         { "websocket_secure", server_network_type::websocket_secure } };

    if (const auto iter = server_network.find(server_network_name);
        iter != server_network.cend())
    {
        return iter->second;
    }

    throw celeritas_error{ "server network type error,name = " + server_network_name };
}

const celeritas::server_network_type_container& celeritas::get_all_server_network_type()
{
    static server_network_type_container container{ server_network_type::tcp,
                                                    server_network_type::http,
                                                    server_network_type::websocket,
                                                    server_network_type::tcp_ssl,
                                                    server_network_type::https,
                                                    server_network_type::websocket_secure };

    return container;
}