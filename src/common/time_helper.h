#pragma once

#include <chrono>

namespace celeritas
{
    class time_helper
    {
    public:
        using class_type = time_helper;
        using local_time_type = std::chrono::local_time<std::chrono::system_clock::duration>;
        using time_point_type = std::chrono::system_clock::time_point;
        using const_time_zone_ptr_type = const std::chrono::time_zone*;
        using local_days_type = std::chrono::local_days;

        [[nodiscard]] static int64_t get_current_milliseconds();

        [[nodiscard]] static int64_t get_current_seconds();

        [[nodiscard]] static int64_t get_start_of_day_milliseconds();

        [[nodiscard]] static int64_t get_start_of_day_milliseconds_with_offset(int64_t milliseconds_offset);

        [[nodiscard]] static int64_t get_start_of_week_milliseconds();

        [[nodiscard]] static int64_t get_start_of_week_milliseconds_with_offset(int64_t milliseconds_offset);

        [[nodiscard]] static int64_t get_start_of_month_milliseconds();

        [[nodiscard]] static int64_t get_start_of_month_milliseconds_with_offset(int64_t milliseconds_offset);

        [[nodiscard]] static int64_t to_milliseconds(const time_point_type& time_point);

        [[nodiscard]] static local_time_type to_local_time(const time_point_type& time_point);

        [[nodiscard]] static const_time_zone_ptr_type get_local_zone();

    private:
        [[nodiscard]] static const_time_zone_ptr_type do_get_local_zone();

        [[nodiscard]] static local_days_type get_current_day_local(const time_point_type& time_point);
    };
}
