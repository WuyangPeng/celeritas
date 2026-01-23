#pragma once

#include <chrono>

namespace celeritas
{
    constexpr auto services_heartbeat_remove_time = 120;
    constexpr auto services_heartbeat_error_time = 60;
    constexpr auto services_heartbeat_warning_time = 10;

    constexpr auto cleanup_seconds = std::chrono::seconds{ 5 };
    constexpr auto check_tcp_clients_seconds = std::chrono::seconds{ 10 };
    constexpr auto service_registry_seconds = std::chrono::seconds{ 5 };

    constexpr auto buffer_pool_seconds = std::chrono::minutes{ 1 };
    constexpr auto buffer_pool_reclaim_seconds = std::chrono::minutes{ 5 };
}
