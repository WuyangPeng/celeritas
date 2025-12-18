#include "calculate_null_red_dot.h"
#include "calculate_red_dot.h"

celeritas::calculate_red_dot::calculate_red_dot(player_state* player_state) noexcept
    : player_state_{ player_state }
{
}

celeritas::calculate_red_dot::calculate_red_dot_shared_ptr celeritas::calculate_red_dot::create(red_dot_type red_type, player_state* player_state)
{
    return std::make_shared<calculate_null_red_dot>(player_state);
}