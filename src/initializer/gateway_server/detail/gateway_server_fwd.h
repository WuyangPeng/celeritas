#pragma once

#include <chrono>

namespace celeritas
{
    class player_check_timer;

    constexpr auto player_check_time = std::chrono::seconds(5);
}