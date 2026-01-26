#pragma once

#include "common/framework/timer_base.h"

namespace celeritas
{
    class player_state_check_timer final : public timer_base
    {
    public:
        using class_type = player_state_check_timer;
        using base_type = timer_base;

        explicit player_state_check_timer(const any_io_executor& any_io_executor, duration_type interval);

    private:
        void execute_timer_task() override;

        any_io_executor any_io_executor_;
    };
}