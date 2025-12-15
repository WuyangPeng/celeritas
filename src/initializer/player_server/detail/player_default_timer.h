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

        explicit player_default_timer(io_context_type& io_context, duration_type interval, default_time_type default_time_type);

    private:
        void execute_timer_task() override;

        io_context_type& io_context_;
        default_time_type default_time_type_;
    };
}