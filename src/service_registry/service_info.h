#pragma once

#include <chrono>
#include <string>

namespace celeritas
{
    class service_info
    {
    public:
        using class_type = service_info;
        using time_point_type = std::chrono::steady_clock::time_point;

        service_info() noexcept = default;

        service_info(std::string instance_id, std::string service_name, std::string host, int port, std::string game_server_id);

        [[nodiscard]] std::string get_instance_id() const;

        [[nodiscard]] std::string get_service_name() const;

        [[nodiscard]] std::string get_host() const;

        [[nodiscard]] int get_port() const;

        [[nodiscard]] std::string get_game_server_id() const;

        [[nodiscard]] time_point_type get_last_heartbeat() const;

        void set_last_heartbeat();

    private:
        std::string instance_id;
        std::string service_name;
        std::string host;
        int port = 0;
        std::string game_server_id;
        time_point_type last_heartbeat;
    };
}
