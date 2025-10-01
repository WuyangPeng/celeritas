#include "server_network_type.h"
#include "common/celeritas_error.h"

#include <map>

celeritas::server_network_type celeritas::get_server_network_type(const std::string& server_network_name)
{
    using server_network_container_type = std::map<std::string, server_network_type>;

    static server_network_container_type server_network{ { "tcp", server_network_type::tcp },
                                                         { "http", server_network_type::http },
                                                         { "websocket", server_network_type::websocket }, };

    const auto iter = server_network.find(server_network_name);
    if (iter != server_network.end())
    {
        return iter->second;
    }

    throw celeritas_error("server_network_type error,name =" + server_network_name);
}