#pragma once

#include "../basic/server_network_type.h"

namespace celeritas
{
    class server_network_config
    {
    public:
        using class_type = server_network_config;

        server_network_config() noexcept = default;

        server_network_config(server_network_type server_network_type, int port) noexcept;

        [[nodiscard]] server_network_type get_server_network_type() const noexcept;

        [[nodiscard]] int get_port() const noexcept;

    private:
        server_network_type server_network_type_ = server_network_type::unknown;
        int port_ = 0;
    };
}
