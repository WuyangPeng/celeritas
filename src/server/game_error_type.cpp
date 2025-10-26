#include "game_error_type.h"

std::string celeritas::get_game_error_description(game_error_type game_error_type)
{
    switch (game_error_type)
    {
        case game_error_type::Unknown:
        {
            return "Unknown error";
        }

        case game_error_type::Success:
        {
            return "Success";
        }
    }

    return "Unknown error";
}