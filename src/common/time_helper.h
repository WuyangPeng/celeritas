#pragma once

#include <chrono>

namespace celeritas
{
    class time_helper
    {
    public:
        using class_type = time_helper;

        [[nodiscard]] static int64_t get_current_milliseconds();

        [[nodiscard]] static int64_t get_current_seconds();
    };
}
