#pragma once

#include <chrono>

namespace celeritas
{
    class player_state_check_timer;

    constexpr auto player_state_check_seconds = std::chrono::minutes(1);
}