#pragma once

#include <string_view>

namespace celeritas
{
    class server_base;
    class celeritas_game_error;
    enum class game_error_type;

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
