#pragma once

#include <string>

namespace celeritas
{
    class service_registry_config
    {
    public:
        using class_type = service_registry_config;

    private:
        std::string host_;
        int port_ = 0;
    };
}



