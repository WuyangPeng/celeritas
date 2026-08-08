#pragma once

#include <chrono>

namespace celeritas
{
    constexpr auto log_flush_interval = std::chrono::seconds{ 5 };
}
