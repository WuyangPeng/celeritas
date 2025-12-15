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
        execute_zero_timer_task();
    }

    set_duration_type(player_default_time_seconds);
}

void celeritas::player_default_timer::execute_zero_timer_task()
{
    const auto now = std::chrono::system_clock::now();

    const auto current_zone = std::chrono::current_zone();

    const auto zone_time = std::chrono::zoned_time{ current_zone, now };

    const auto local_time = zone_time.get_local_time();

    const auto today_local = std::chrono::floor<std::chrono::days>(local_time);

    execute_week_timer_task(today_local);
    execute_month_timer_task(today_local);
}

void celeritas::player_default_timer::execute_week_timer_task(const time_point_type& today_local)
{
    switch (const std::chrono::weekday today_weekday{ today_local };
        today_weekday.c_encoding())
    {
        case 0:
        {
            boost::asio::co_spawn(io_context_,
                                  player_manager::get_instance().time_callback(default_time_type::weekly_7_0),
                                  boost::asio::detached);
            break;
        }
        case 1:
        {
            boost::asio::co_spawn(io_context_,
                                  player_manager::get_instance().time_callback(default_time_type::weekly_1_0),
                                  boost::asio::detached);
            break;
        }
        default:
        {
            break;
        }
    }
}

void celeritas::player_default_timer::execute_month_timer_task(const time_point_type& today_local)
{
    switch (const std::chrono::year_month_day year_month_day_local{ today_local };
        static_cast<unsigned int>(year_month_day_local.day()))
    {
        case 1:
        {
            boost::asio::co_spawn(io_context_,
                                  player_manager::get_instance().time_callback(default_time_type::monthly_1_0),
                                  boost::asio::detached);
            break;
        }
        default:
        {
            break;
        };
    }
}