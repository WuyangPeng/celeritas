#include "player_timer.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "player/time/player_time_component.h"

celeritas::player_timer::player_timer(const any_io_executor& any_io_executor, const duration_type interval, const player_timer_component_shared_ptr& player_timer_component)
    : base_type{ any_io_executor, interval, true }, any_io_executor_{ any_io_executor }, player_timer_component_{ player_timer_component }
{
}

void celeritas::player_timer::execute_timer_task()
{
    if (const auto player_timer_component = player_timer_component_.lock();
        player_timer_component != nullptr)
    {
        boost::asio::co_spawn(any_io_executor_,
                              noexcept_safe_call_and_log_awaitable([player_timer_component = player_timer_component]() {
                                                                       return player_timer_component->time_callback();
                                                                   },
                                                                   player_channel,
                                                                   "time callback error:"),

                              boost::asio::detached);
    }
}