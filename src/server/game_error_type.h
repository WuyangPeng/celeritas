#pragma once

#include <string>

namespace celeritas
{
    enum class game_error_type
    {
        unknown = 0,
        success = 1,
    };

    [[nodiscard]] std::string get_game_error_description(game_error_type game_error);
}