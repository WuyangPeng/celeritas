#pragma once

#include <string_view>

namespace celeritas
{
    class celeritas_error;
    class logger;
    class buffer_pool_data;
    class buffer_pool;
    class buffer_guard;

    constexpr std::string_view default_channel{ "default" };
    constexpr std::string_view unregistered_channel{ "unregistered" };
    constexpr std::string_view channel{ "channel" };
    constexpr std::string_view common_channel{ "common" };
    constexpr std::string_view network_channel{ "network" };
    constexpr std::string_view worker_pool_channel{ "worker_pool" };
    constexpr std::string_view database_channel{ "database" };
    constexpr std::string_view initializer_channel{ "initializer" };

    constexpr std::string_view service_registry_channel{ "service_registry" };
}
