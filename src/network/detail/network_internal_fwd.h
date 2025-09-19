#pragma once

#include <chrono>

namespace celeritas
{
    constexpr auto max_message_size = 16 * 1024 * 1024;  // 16 MB
    constexpr auto timeout_seconds = std::chrono::seconds(60);
}