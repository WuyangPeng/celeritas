#pragma once

#include <chrono>

namespace celeritas
{
    constexpr auto timeout_seconds = std::chrono::seconds{ 60 };
    constexpr std::string_view response_suffix = "_response";
    constexpr auto max_queue_size = 5000;
}