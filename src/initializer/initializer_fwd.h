#pragma once

#include <string_view>

namespace celeritas
{
    class initializer;
    class configuration_loader;
    class initializer_factory;
    class application_loader;
    class resource_loader;
    class daemon;
    class crash;
    class register_handler_helper;

    class service_registry_resource_loader;
    class service_registry_configuration_loader;
    class service_registry_application_loader;

    class auth_resource_loader;
    class auth_configuration_loader;
    class auth_application_loader;

    class gateway_resource_loader;
    class gateway_configuration_loader;
    class gateway_application_loader;

    class player_resource_loader;
    class player_configuration_loader;
    class player_application_loader;

    class logic_resource_loader;
    class logic_configuration_loader;
    class logic_application_loader;

    class game_resource_loader;
    class game_configuration_loader;
    class game_application_loader;

    class battle_resource_loader;
    class battle_configuration_loader;
    class battle_application_loader;

    class payment_resource_loader;
    class payment_configuration_loader;
    class payment_application_loader;

    class admin_resource_loader;
    class admin_configuration_loader;
    class admin_application_loader;

    class cross_resource_loader;
    class cross_configuration_loader;
    class cross_application_loader;

    class chat_resource_loader;
    class chat_configuration_loader;
    class chat_application_loader;

    class log_resource_loader;
    class log_configuration_loader;
    class log_application_loader;

    class server_base;
    enum class account_type;
    enum class account_status_type;

    constexpr std::string_view generate_handler_tools_type = "generate_handler_tools";
    constexpr std::string_view generate_database_tools_type = "generate_database_tools";
    constexpr std::string_view service_registry_type = "service_registry";
    constexpr std::string_view auth_type = "auth";
    constexpr std::string_view gateway_type = "gateway";
    constexpr std::string_view player_type = "player";
    constexpr std::string_view logic_type = "logic";
    constexpr std::string_view game_type = "game";
    constexpr std::string_view payment_type = "payment";
    constexpr std::string_view admin_type = "admin";
    constexpr std::string_view cross_type = "cross";
    constexpr std::string_view chat_type = "chat";
    constexpr std::string_view battle_type = "battle";
    constexpr std::string_view log_type = "log";
}