#include "player_state_check_timer.h"
#include "player/component/player_manager.h"

celeritas::player_state_check_timer::player_state_check_timer(io_context_type& io_context, duration_type interval)
    : base_type{ io_context, interval }, io_context_{ io_context }
{
}

void celeritas::player_state_check_timer::execute_timer_task()
{
    boost::asio::co_spawn(io_context_,
                          player_manager::get_instance().save_db(),
                          boost::asio::detached);
}