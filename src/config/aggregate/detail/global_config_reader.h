#pragma once

#include "config/local/global_config.h"

#include <memory>
#include <string>

namespace celeritas
{
    class global_config_reader
    {
    public:
        using class_type = global_config_reader;
        using const_global_shared_ptr = std::shared_ptr<const global_config>;

        [[nodiscard]] static const_global_shared_ptr load_config(const std::string& filename);
    };
}