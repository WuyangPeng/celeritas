#pragma once

#include <string>

namespace celeritas
{
    enum class game_error_type
    {
        Unknown = 0,
        Success = 1,
    };

    [[nodiscard]] std::string get_game_error_description(game_error_type game_error_type);
}