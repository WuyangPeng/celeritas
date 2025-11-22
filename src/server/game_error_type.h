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
        redis_error = 101,
    };

    [[nodiscard]] std::string get_game_error_description(game_error_type game_error);
}