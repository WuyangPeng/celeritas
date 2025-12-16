#pragma once

#include "common/timer_base.h"
#include "player/player_fwd.h"

namespace celeritas
{
    class player_default_timer final : public timer_base
    {
    public:
        using class_type = player_default_timer;
        using base_type = timer_base;

        explicit player_default_timer(io_context_type& io_context, duration_type interval, int local_hours);

    private:
        using time_point_type = std::chrono::time_point<std::chrono::local_t, std::chrono::days>;

        void execute_timer_task() override;

        void execute_zero_timer_task();

        void execute_week_timer_task(const time_point_type& today_local);

        void execute_month_timer_task(const time_point_type& today_local);

        io_context_type& io_context_;
        int local_hours_;
    };
}