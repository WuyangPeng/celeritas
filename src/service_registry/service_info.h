#pragma once

#include "protocol_port.h"
#include "message/message_handler/health_check_level_type.h"

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
                     protocol_port_container protocol_port,
                     int64_t start_server_time);

        [[nodiscard]] std::string get_instance_id() const;

        [[nodiscard]] std::string get_service_name() const;

        [[nodiscard]] std::string get_host() const;

        [[nodiscard]] std::string get_game_server_id() const;

        [[nodiscard]] time_point_type get_last_heartbeat() const;

        void set_last_heartbeat(int64_t start_server_time);

        [[nodiscard]] int get_port(server_network_type server_network_type) const;

        [[nodiscard]] health_check_level_type get_health_check_level_type() const noexcept;

        void set_health_check_level_type(health_check_level_type health_check_level);

        [[nodiscard]] int64_t get_start_server_time() const;

    private:
        std::string instance_id_;
        std::string service_name_;
        std::string host_;
        std::string game_server_id_;
        time_point_type last_heartbeat_;
        protocol_port_container protocol_port_;
        health_check_level_type health_check_level_ = health_check_level_type::health;
        int64_t start_server_time_ = 0;
    };
}
