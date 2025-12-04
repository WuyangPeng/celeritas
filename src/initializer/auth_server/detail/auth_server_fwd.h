#pragma once

#include <chrono>

namespace celeritas
{
    class gateway_check_timer;
    class auth_health_check_timer;

    constexpr auto gateway_check_time = std::chrono::seconds(5);
}