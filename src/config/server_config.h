#pragma once
#include <string>

namespace celeritas
{
    class server_config
    {
    public:
        using class_type = server_config;

        server_config() noexcept = default;

        server_config(std::string service_name, std::string host, int tcp_port, std::string game_server_id) noexcept;

    private:
        std::string service_name_;
        std::string host_;
        int tcp_port_ = 0;
        std::string game_server_id_;
    };
}
