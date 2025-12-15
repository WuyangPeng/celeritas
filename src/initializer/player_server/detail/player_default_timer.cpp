#include "player_default_timer.h"
#include "player_server_fwd.h"
#include "player/component/player_manager.h"
#include "player/time/default_time_type.h"

celeritas::player_default_timer::player_default_timer(io_context_type& io_context, const duration_type interval, const default_time_type default_time_type)
    : base_type{ io_context, interval }, io_context_{ io_context }, default_time_type_{ default_time_type }
{
}

void celeritas::player_default_timer::execute_timer_task()
{
    boost::asio::co_spawn(io_context_,
                          player_manager::get_instance().time_callback(default_time_type_),
                          boost::asio::detached);

    if (default_time_type_ == default_time_type::daily_0)
    {
    }

    set_duration_type(player_default_time_seconds);
}