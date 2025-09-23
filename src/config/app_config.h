#pragma once

#include "database_config.h"
#include "health_check_url_config.h"
#include "logger_config.h"
#include "server_config.h"
#include "service_registry_config.h"

#include <vector>

namespace celeritas
{
    class app_config
    {
    public:
        using class_type = app_config;

    private:
        service_registry_config service_registry_;
        server_config server_;
        health_check_url_config health_check_url_;
        std::vector<database_config> database_;
        std::vector<logger_config> logger_;
    };
}