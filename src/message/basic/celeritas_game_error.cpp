#include "celeritas_game_error.h"

celeritas::celeritas_game_error::celeritas_game_error(const game_error_type game_error_type)
    : base_type{ get_game_error_description(game_error_type) }
{
}

celeritas::celeritas_game_error::celeritas_game_error(const game_error_type game_error_type, const std::string& error)
    : base_type{ "{}:{}", get_game_error_description(game_error_type), error }
{
}