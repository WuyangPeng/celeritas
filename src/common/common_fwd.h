#pragma once

#include <string_view>

namespace celeritas
{
    class celeritas_error;
    class logger;
    class buffer_pool_data;
    class buffer_pool;
    class buffer_guard;
    class command_line_config;
    class random_helper;
    class timer_base;

    constexpr std::string_view channel{ "channel" };
    constexpr std::string_view default_channel{ "default" };
    constexpr std::string_view unregistered_channel{ "unregistered" };
    constexpr std::string_view common_channel{ "common" };
    constexpr std::string_view network_channel{ "network" };
    constexpr std::string_view worker_pool_channel{ "worker_pool" };
    constexpr std::string_view database_channel{ "database" };
    constexpr std::string_view initializer_channel{ "initializer" };
    constexpr std::string_view message_channel{ "message" };
    constexpr std::string_view config_channel{ "config" };
    constexpr std::string_view server_channel{ "server" };
    constexpr std::string_view service_registry_channel{ "service_registry" };

    constexpr std::string_view log_function{ "function" };
    constexpr std::string_view log_file{ "file" };
    constexpr std::string_view log_line{ "line" };

    constexpr std::string_view config_file_path_command_line{ "config_file_path" };
}
