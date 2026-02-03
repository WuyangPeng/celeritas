#pragma once

#include <chrono>

namespace celeritas
{
    constexpr auto cleanup_database_timer = std::chrono::minutes{ 30 };
    constexpr auto cleanup_database_expired_timer = std::chrono::hours{ 1 };

    constexpr auto redis_cursor_one_request_size = 100;

    constexpr std::string_view mysql_auth_db_name = "mysql_auth_db";
    constexpr std::string_view mongo_auth_db_name = "mongo_auth_db";
    constexpr std::string_view mysql_player_db_name = "mysql_player_db";
    constexpr std::string_view mongo_player_db_name = "mongo_player_db";
    constexpr std::string_view mongo_admin_db_name = "mongo_admin_db";
    constexpr std::string_view mysql_config_db_name = "mysql_config_db";
    constexpr std::string_view payment_db_name = "mysql_payment_db";
    constexpr std::string_view redis_db_name = "redis_db";

    constexpr std::string_view time_refresh_db_name = "time_refresh";

    constexpr std::string_view redis_ok = "OK";
}
