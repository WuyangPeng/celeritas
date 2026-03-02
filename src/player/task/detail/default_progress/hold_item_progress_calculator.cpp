#include "hold_item_progress_calculator.h"
#include "player/component/player_state.h"
#include "player/item/player_item_component.h"

celeritas::hold_item_progress_calculator::hold_item_progress_calculator(player_state* player_state) noexcept
    : base_type{ player_state }
{
}

int celeritas::hold_item_progress_calculator::calculate(const config::task_event_type task_event_type, const int target_id) const
{
    if (task_event_type != config::task_event_type::hold_item)
    {
        return 0;
    }

    return player_state_->get_component<player_item_component>()->get_count(target_id);
}



