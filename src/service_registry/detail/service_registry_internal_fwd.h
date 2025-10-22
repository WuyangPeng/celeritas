#pragma once

#include <chrono>

namespace celeritas
{
    constexpr auto services_heartbeat_remove_time = 120;
    constexpr auto services_heartbeat_error_time = 60;
    constexpr auto services_heartbeat_warning_time = 10;

    constexpr auto cleanup_timer = std::chrono::seconds(5);
    constexpr auto check_tcp_clients_timer = std::chrono::seconds(10);
    constexpr auto service_registry_timer = std::chrono::seconds(5);

    class service_registry_impl;
}
