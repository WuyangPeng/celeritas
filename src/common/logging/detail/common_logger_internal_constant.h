#pragma once

#include <string_view>

namespace celeritas
{
    constexpr std::string_view log_daily_suffix{ "_%Y%m%d_%N" };
    constexpr std::string_view log_date_time_format{ "%Y-%m-%d %H:%M:%S.%f" };
}
