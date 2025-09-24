#pragma once

#include "config/server_config.h"

namespace celeritas
{
    class server_config_reader
    {
    public:
        using class_type = server_config_reader;

        static server_config load_config(const std::string& filename);
    };
}
