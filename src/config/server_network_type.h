#pragma once

#include <string>

namespace celeritas
{
    enum class server_network_type
    {
        unknown,
        tcp,
        http,
        websocket,
    };

    [[nodiscard]] server_network_type get_server_network_type(const std::string& server_network_name);
}
