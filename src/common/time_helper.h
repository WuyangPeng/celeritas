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

        [[nodiscard]] static int64_t get_start_of_day_milliseconds();

        [[nodiscard]] static int64_t get_milliseconds_with_offset(int64_t milliseconds_offset);

        [[nodiscard]] static int64_t get_start_of_week_milliseconds();

        [[nodiscard]] static int64_t get_start_of_week_milliseconds_with_offset(int64_t milliseconds_offset);

        [[nodiscard]] static int64_t get_start_of_month_milliseconds();

        [[nodiscard]] static int64_t get_start_of_month_milliseconds_with_offset(int64_t milliseconds_offset);

        [[nodiscard]] static int64_t to_milliseconds(const std::chrono::system_clock::time_point& time_point);
    };
}
