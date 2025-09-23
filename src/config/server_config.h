#pragma once
#include <string>

namespace celeritas
{
    class server_config
    {
    public:
        using class_type = server_config;

    private:
        std::string service_name_;
        std::string host_;
        int port_ = 0;
        std::string game_server_id_;
    };
}
