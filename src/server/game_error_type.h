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

        // auth错误
        account_error = 1000,
        no_guest_account = 1001,
        account_bound = 1002,

        // 数据库错误
        redis_error = 2000,
        mysql_error = 2001,
    };

    [[nodiscard]] std::string get_game_error_description(game_error_type game_error);
}