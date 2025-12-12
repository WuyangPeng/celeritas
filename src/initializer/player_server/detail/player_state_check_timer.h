#pragma once
#include "common/timer_base.h"

namespace celeritas
{
    class player_state_check_timer final : public timer_base
    {
    public:
        using class_type = player_state_check_timer;
        using base_type = timer_base;

        explicit player_state_check_timer(io_context_type& io_context, duration_type interval);

    private:
        void execute_timer_task() override;

        io_context_type& io_context_;
    };
}