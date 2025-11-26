#pragma once

#include <string_view>

namespace celeritas
{
    class celeritas_error;
    class random_helper;
    class time_helper;
    class snowflake_generator;
    class hmac_sha256;
    class command_line_config;

    class logger;

    class buffer_pool_data;
    class buffer_pool;
    class buffer_guard;

    class thread_safe_queue;
    class worker_pool;

    class timer_base;
    class session;
    class resource_loader_base;
    class application_loader_base;
    class celeritas_main;

    constexpr std::string_view channel{ "channel" };
    constexpr std::string_view default_channel{ "default" };
    constexpr std::string_view unregistered_channel{ "unregistered" };
    constexpr std::string_view common_channel{ "common" };
    constexpr std::string_view config_channel{ "config" };
    constexpr std::string_view message_channel{ "message" };
    constexpr std::string_view database_channel{ "database" };
    constexpr std::string_view network_channel{ "network" };
    constexpr std::string_view service_registry_channel{ "service_registry" };
    constexpr std::string_view auth_channel{ "auth" };
    constexpr std::string_view handler_channel{ "handler" };
    constexpr std::string_view initializer_channel{ "initializer" };
    constexpr std::string_view server_channel{ "server" };

    constexpr std::string_view log_function{ "function" };
    constexpr std::string_view log_file{ "file" };
    constexpr std::string_view log_line{ "line" };

    constexpr std::string_view config_file_path_command_line{ "config_file_path" };

    constexpr auto worker_id_bits = 8;
    constexpr auto datacenter_id_bits = 5;
    constexpr auto sequence_bits = 6;
    constexpr auto max_worker_id = (1 << worker_id_bits) - 1;
    constexpr auto max_datacenter_id = (1 << datacenter_id_bits) - 1;

    // 2025-04-12 06:20:00 UTC
    constexpr auto epoch = 1744438800000;

    constexpr auto worker_id_shift = sequence_bits;
    constexpr auto datacenter_id_shift = sequence_bits + worker_id_bits;
    constexpr auto timestamp_left_shift = sequence_bits + worker_id_bits + datacenter_id_bits;
    constexpr auto sequence_mask = (1 << sequence_bits) - 1;

    constexpr auto max_clock_skew_ms = 5;

    constexpr auto milliseconds = 1000;
    constexpr auto minute = 60 * milliseconds;
}
