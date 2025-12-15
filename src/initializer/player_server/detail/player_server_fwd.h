#pragma once

#include <chrono>

namespace celeritas
{
    class player_state_check_timer;
    class player_default_timer;

    constexpr auto player_state_check_seconds = std::chrono::minutes(1);
    constexpr auto player_default_time_seconds = std::chrono::hours(1);
}