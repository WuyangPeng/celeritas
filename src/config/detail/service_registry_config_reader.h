#pragma once

#include "config/service_registry_config.h"

namespace celeritas
{
    class service_registry_config_reader
    {
    public:
        using class_type = service_registry_config_reader;

        static service_registry_config load_config(const std::string& filename);
    };
}
