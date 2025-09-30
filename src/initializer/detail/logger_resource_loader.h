#pragma once

#include "config/logger_config.h"
#include "config/logger_level_config.h"

namespace celeritas
{
    class logger_resource_loader
    {
    public:
        using class_type = logger_resource_loader;

        static void loader_level_config(const logger_level_config& logger_level_config);

        static void loader_logger(const logger_config& logger_config);
    };
}