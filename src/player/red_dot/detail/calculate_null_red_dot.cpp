#include "calculate_null_red_dot.h"

celeritas::calculate_null_red_dot::calculate_null_red_dot(player_state* player_state)
    : base_type{ player_state }
{
}

int celeritas::calculate_null_red_dot::get_red_dot_value() const
{
    return 0;
}