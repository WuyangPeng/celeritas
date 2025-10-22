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

        service_info(std::string instance_id,
                     std::string service_name,
                     std::string host,
                     std::string game_server_id,
                     int tcp_port,
                     int http_port,
                     int websocket_port,
                     int tcp_ssl_port,
                     int https_port,
                     int websocket_secure_port);

        [[nodiscard]] std::string get_instance_id() const;

        [[nodiscard]] std::string get_service_name() const;

        [[nodiscard]] std::string get_host() const;

        [[nodiscard]] std::string get_game_server_id() const;

        [[nodiscard]] time_point_type get_last_heartbeat() const;

        void set_last_heartbeat();

        [[nodiscard]] int get_tcp_port() const;

        [[nodiscard]] int get_tcp_ssl_port() const;

        [[nodiscard]] int get_http_port() const;

        [[nodiscard]] int get_https_port() const;

        [[nodiscard]] int get_websocket_port() const;

        [[nodiscard]] int get_websocket_secure_port() const;

    private:
        std::string instance_id_;
        std::string service_name_;
        std::string host_;
        std::string game_server_id_;
        time_point_type last_heartbeat_;
        int tcp_port_ = 0;
        int http_port_ = 0;
        int websocket_port_ = 0;
        int tcp_ssl_port_ = 0;
        int https_port_ = 0;
        int websocket_secure_port_ = 0;
    };
}
