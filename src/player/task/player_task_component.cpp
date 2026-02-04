#include "player_task_component.h"
#include "detail/component/daily_task.h"
#include "detail/component/main_task.h"

celeritas::player_task_component::player_task_component(player_state* player_state) noexcept
    : base_type{ get_player_component_type(), player_state },
      tasks_{ std::make_shared<main_task>(player_state),
              std::make_shared<daily_task>(player_state) }
{
}
