#pragma once

#include "server_network_type.h"

#include <string>

namespace celeritas
{
    class server_network_config
    {
    public:
        using class_type = server_network_config;

        server_network_config() noexcept = default;

        server_network_config(server_network_type server_network_type, std::string host, int port) noexcept;

        [[nodiscard]] server_network_type get_server_network_type() const noexcept;

        [[nodiscard]] int get_port() const noexcept;

    private:
        server_network_type server_network_type_;
        std::string host_;
        int port_ = 0;
    };
}
