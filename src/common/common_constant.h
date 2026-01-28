#pragma once

#include <string_view>

namespace celeritas
{
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
    constexpr std::string_view payment_channel{ "payment" };
    constexpr std::string_view admin_channel{ "admin" };
    constexpr std::string_view chat_channel{ "chat" };
    constexpr std::string_view gateway_channel{ "gateway" };
    constexpr std::string_view player_channel{ "player" };
    constexpr std::string_view logic_channel{ "logic" };
    constexpr std::string_view game_channel{ "game" };
    constexpr std::string_view battle_channel{ "battle" };
    constexpr std::string_view cross_channel{ "cross" };
    constexpr std::string_view log_channel{ "log" };
    constexpr std::string_view handler_channel{ "handler" };
    constexpr std::string_view initializer_channel{ "initializer" };
    constexpr std::string_view server_channel{ "server" };

    constexpr std::string_view log_function{ "function" };
    constexpr std::string_view log_file{ "file" };
    constexpr std::string_view log_line{ "line" };
 constexpr std::string_view logger_path = "log";
    constexpr std::string_view logger_extension = ".log";

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

    constexpr auto milliseconds = 1000LL;
    constexpr auto minute_seconds = 60LL;
    constexpr auto minute_milliseconds = minute_seconds * milliseconds;
    constexpr auto hour_seconds = minute_seconds * minute_seconds;
    constexpr auto hour_milliseconds = hour_seconds * milliseconds;
    constexpr auto day_hour = 24LL;
    constexpr auto day_seconds = hour_seconds * day_hour;
    constexpr auto day_milliseconds = day_seconds * milliseconds;
    constexpr auto week = 7LL;
    constexpr auto week_milliseconds = day_milliseconds * week;
    constexpr auto min_month = 28LL;
    constexpr auto max_month = 31LL;
    constexpr auto max_month_milliseconds = day_milliseconds * max_month;

    constexpr auto http_request_timestamp_expired = minute_milliseconds * 5;

    constexpr auto one_kibibyte = 1024;
    constexpr auto one_mebibyte = one_kibibyte * one_kibibyte;
}
