#pragma once

#include "config/config_fwd.h"

#include <cstdint>
#include <string>

namespace celeritas
{
    class session_route
    {
    public:
        using class_type = session_route;

        session_route(server_network_type server_network_type, int64_t session_id, std::string instance_id) noexcept;

        [[nodiscard]] server_network_type get_server_network_type() const noexcept;

        [[nodiscard]] int64_t get_session_id() const noexcept;

        [[nodiscard]] std::string get_instance_id() const;

    private:
        server_network_type server_network_type_;
        int64_t session_id_;
        std::string instance_id_;
    };
}