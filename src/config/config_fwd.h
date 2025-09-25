#pragma once

#include <string_view>

namespace celeritas
{
    class service_registry_config;
    class server_config;
    class health_check_url_config;
    class database_config;
    class logger_config;
    class app_config;

    constexpr std::string_view default_service_registry_host = "127.0.0.1";
    constexpr auto default_service_registry_port = 35000;
    constexpr auto default_health_check_interval = 30;
    constexpr auto default_health_check_timeout = 5;

    constexpr std::string_view config_path = "config";
    constexpr std::string_view service_registry_xml = "service_registry.xml";
    constexpr std::string_view server_xml = "server.xml";
    constexpr std::string_view health_check_url_xml = "health_check_url.xml";
}