#include "default_progress_calculator.h"

celeritas::default_progress_calculator::default_progress_calculator(player_state* player_state) noexcept
    : player_state_{ player_state }
{
}