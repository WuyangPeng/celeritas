#pragma once

#include <string>

namespace celeritas
{
    class service_registry_config
    {
    public:
        using class_type = service_registry_config;

    private:
        std::string ipc_name;
        std::string host;
        int port = 0;
    };
}



