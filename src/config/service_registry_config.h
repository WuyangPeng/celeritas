#pragma once

#include "config_fwd.h"

#include <string>

namespace celeritas
{
    class service_registry_config
    {
    public:
        using class_type = service_registry_config;

        service_registry_config() noexcept = default;

        service_registry_config(std::string host, int port) noexcept;

    private:
        std::string host_ = default_service_registry_host.data();
        int port_ = default_service_registry_port;
    };
}



