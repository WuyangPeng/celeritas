#pragma once

#include "common/common_fwd.h"

namespace celeritas
{
    enum class default_time_type
    {
        // 每日重置。
        daily_0 = (hour_seconds * 0) * 10 + 1,
        daily_1 = (hour_seconds * 1) * 10 + 1,
        daily_2 = (hour_seconds * 2) * 10 + 1,
        daily_3 = (hour_seconds * 3) * 10 + 1,
        daily_4 = (hour_seconds * 4) * 10 + 1,
        daily_5 = (hour_seconds * 5) * 10 + 1,
        daily_6 = (hour_seconds * 6) * 10 + 1,
        daily_7 = (hour_seconds * 7) * 10 + 1,
        daily_8 = (hour_seconds * 8) * 10 + 1,
        daily_9 = (hour_seconds * 9) * 10 + 1,
        daily_10 = (hour_seconds * 10) * 10 + 1,
        daily_11 = (hour_seconds * 11) * 10 + 1,
        daily_12 = (hour_seconds * 12) * 10 + 1,
        daily_13 = (hour_seconds * 13) * 10 + 1,
        daily_14 = (hour_seconds * 14) * 10 + 1,
        daily_15 = (hour_seconds * 15) * 10 + 1,
        daily_16 = (hour_seconds * 16) * 10 + 1,
        daily_17 = (hour_seconds * 17) * 10 + 1,
        daily_18 = (hour_seconds * 18) * 10 + 1,
        daily_19 = (hour_seconds * 19) * 10 + 1,
        daily_20 = (hour_seconds * 20) * 10 + 1,
        daily_21 = (hour_seconds * 21) * 10 + 1,
        daily_22 = (hour_seconds * 22) * 10 + 1,
        daily_23 = (hour_seconds * 23) * 10 + 1,

        // 每周重置。
        weekly_1_0 = ((day_seconds * 0) + (hour_seconds * 0)) * 10 + 2,
        weekly_7_0 = ((day_seconds * 6) + (hour_seconds * 0)) * 10 + 2,

        // 每月重置。
        monthly_1_0 = ((day_seconds * 0) + (hour_seconds * 0)) * 10 + 3,
    };
}