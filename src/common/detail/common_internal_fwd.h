#pragma once

#include <string>

namespace celeritas
{
    class logger_impl;
    class buffer_pool_impl;
    class buffer_pool_bucket;

    constexpr auto max_idle_per_size = 16;
}
