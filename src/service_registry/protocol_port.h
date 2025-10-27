#pragma once

#include "config/config_fwd.h"

namespace celeritas
{
    class protocol_port
    {
    public:
        using class_type = protocol_port;

        protocol_port(server_network_type server_network_type, int port);

        [[nodiscard]] server_network_type get_server_network_type() const noexcept;

        [[nodiscard]] int get_port() const noexcept;

    private:
        server_network_type server_network_type_;
        int port_;
    };
}