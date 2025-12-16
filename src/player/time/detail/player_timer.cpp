#include "player_timer.h"
#include "player/time/player_time_component.h"

celeritas::player_timer::player_timer(io_context_type& io_context, const duration_type interval, const player_timer_component_shared_ptr& player_timer_component)
    : base_type{ io_context, interval, true }, io_context_{ io_context }, player_timer_component_{ player_timer_component }
{
}

void celeritas::player_timer::execute_timer_task()
{
    if (const auto player_timer_component = player_timer_component_.lock();
        player_timer_component != nullptr)
    {
        boost::asio::co_spawn(io_context_,
                              player_timer_component->time_callback(),
                              boost::asio::detached);
    }
}