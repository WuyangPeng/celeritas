#include "kill_monster_progress_calculator.h"
#include "player/component/player_state.h"

celeritas::kill_monster_progress_calculator::kill_monster_progress_calculator(player_state* player_state) noexcept
    : base_type{ player_state }
{
}

int celeritas::kill_monster_progress_calculator::calculate(const config::task_event_type task_event_type, int target_id) const
{
    if (task_event_type != config::task_event_type::kill_monster)
    {
        return 0;
    }

    return 0;
}


