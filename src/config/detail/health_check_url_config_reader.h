#pragma once

#include "config/health_check_url_config.h"

namespace celeritas
{
    class health_check_url_config_reader
    {
    public:
        using class_type = health_check_url_config_reader;

        static health_check_url_config load_config(const std::string& filename);
    };
}
