#pragma once

#include "config/global_config.h"

#include <string>

namespace celeritas
{
    class global_config_reader
    {
    public:
        using class_type = global_config_reader;

        [[nodiscard]] static global_config load_config(const std::string& filename);
    };
}