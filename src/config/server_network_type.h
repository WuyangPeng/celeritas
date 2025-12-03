#pragma once

#include <string>
#include <vector>

namespace celeritas
{
    enum class server_network_type
    {
        unknown,
        tcp,
        http,
        websocket,
        tcp_ssl,
        https,
        websocket_secure,
    };

    [[nodiscard]] server_network_type get_server_network_type(const std::string& server_network_name);

    using server_network_type_container = std::vector<server_network_type>;

    [[nodiscard]] const server_network_type_container& get_all_server_network_type();
}
