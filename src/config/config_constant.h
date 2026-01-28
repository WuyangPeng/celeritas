#pragma once

#include <string_view>

namespace celeritas
{
    constexpr std::string_view default_service_registry_host = "127.0.0.1";
    constexpr auto default_service_registry_port = 35000;
    constexpr auto default_health_check_interval = 30;
    constexpr auto default_health_check_timeout = 5;
    constexpr auto default_logger_rotation_size = 50;
    constexpr auto default_database_min_connections = 4;
    constexpr auto default_database_max_connections = 10;
    constexpr auto default_database_timeout_seconds = 5;
    constexpr auto default_database_expire_seconds = 86400;
    constexpr auto default_worker_pool_size = 4;
    constexpr auto min_worker_pool_size = 1;

    constexpr std::string_view config_path = "config";
    constexpr std::string_view bin_path = "bin";

    constexpr std::string_view service_registry_xml = "service_registry.xml";
    constexpr std::string_view server_xml = "server.xml";
    constexpr std::string_view health_check_url_xml = "health_check_url.xml";
    constexpr std::string_view databases_xml = "databases.xml";
    constexpr std::string_view loggers_xml = "loggers.xml";
    constexpr std::string_view global_xml = "global.xml";
}