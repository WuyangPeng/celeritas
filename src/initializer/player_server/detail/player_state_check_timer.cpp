#include "player_state_check_timer.h"
#include "player/component/player_manager.h"

celeritas::player_state_check_timer::player_state_check_timer(const any_io_executor& any_io_executor, duration_type interval)
    : base_type{ any_io_executor, interval }, any_io_executor_{ any_io_executor }
{
}

void celeritas::player_state_check_timer::execute_timer_task()
{
    player_manager::get_instance().save_db();
}