#pragma once

#include "config/local/health_check_url_config.h"

#include <memory>

namespace celeritas
{
    class health_check_url_config_reader
    {
    public:
        using class_type = health_check_url_config_reader;
        using const_health_check_url_config_shared_ptr = std::shared_ptr<const health_check_url_config>;

        static const_health_check_url_config_shared_ptr load_config(const std::string& filename);
    };
}
