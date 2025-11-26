#pragma once

namespace celeritas
{
    class logger_impl;
    class logger_container;
    class logger_console;
    class logger_support;
    class logger_level;

    class buffer_pool_impl;
    class buffer_pool_bucket;

    constexpr auto max_idle_per_size = 16;
}
