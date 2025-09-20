#pragma once

#include <string>

namespace celeritas
{
    class logger_impl;
    class buffer_pool_impl;
    class buffer_pool_bucket;

    constexpr std::string_view default_channel{ "default" };
    constexpr std::string_view unregistered_channel{ "unregistered" };
    constexpr std::string_view channel{ "channel" };
    constexpr std::string_view common_channel{ "common" };
    constexpr std::string_view network_channel{ "network" };
    constexpr std::string_view worker_pool_channel{ "worker_pool" };

    constexpr auto max_idle_per_size = 16;
}
