#pragma once

#include <string>
#include <chrono>

namespace celeritas
{
    class service_info
    {
    public:
        service_info() noexcept = default;

        service_info(std::string instance_id, std::string service_name, std::string host, int port, std::string game_server_id);

        [[nodiscard]] std::string get_instance_id() const;

        [[nodiscard]] std::string get_service_name() const;

        [[nodiscard]] std::string get_host() const;

        [[nodiscard]] int get_port() const;

        [[nodiscard]] std::string get_game_server_id() const;

        [[nodiscard]] std::chrono::steady_clock::time_point get_last_heartbeat() const;

    private:
        std::string instance_id;
        std::string service_name;
        std::string host;
        int port = 0;
        std::string game_server_id;
        std::chrono::steady_clock::time_point last_heartbeat;
    };
}
