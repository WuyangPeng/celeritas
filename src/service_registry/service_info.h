#pragma once

#include "protocol_port.h"

#include <chrono>
#include <string>

namespace celeritas
{
    class service_info
    {
    public:
        using class_type = service_info;
        using time_point_type = std::chrono::steady_clock::time_point;
        using protocol_port_container = std::vector<protocol_port>;

        service_info() noexcept = default;

        service_info(std::string instance_id,
                     std::string service_name,
                     std::string host,
                     std::string game_server_id,
                     protocol_port_container protocol_port);

        [[nodiscard]] std::string get_instance_id() const;

        [[nodiscard]] std::string get_service_name() const;

        [[nodiscard]] std::string get_host() const;

        [[nodiscard]] std::string get_game_server_id() const;

        [[nodiscard]] time_point_type get_last_heartbeat() const;

        void set_last_heartbeat();

        [[nodiscard]] int get_port(server_network_type server_network_type) const;

    private:
        std::string instance_id_;
        std::string service_name_;
        std::string host_;
        std::string game_server_id_;
        time_point_type last_heartbeat_;
        protocol_port_container protocol_port_;
    };
}
