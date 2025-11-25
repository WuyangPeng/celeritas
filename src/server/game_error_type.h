#pragma once

#include <string>

namespace celeritas
{
    enum class game_error_type
    {
        unknown = 0,
        success = 1,

        // 通用错误
        invalid_parameter = 100,
        timestamp_expired = 101,
        sign_error = 102,
        sent_too_frequently = 103,
        code_expired = 104,
        code_error = 105,
        account_error = 106,

        // 数据库错误
        redis_error = 1000,
        mysql_error = 1001,
    };

    [[nodiscard]] std::string get_game_error_description(game_error_type game_error);
}