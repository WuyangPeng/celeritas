#pragma once

#include "common/timer_base.h"
#include "player/player_fwd.h"

namespace celeritas
{
    class player_timer final : public timer_base
    {
    public:
        using class_type = player_timer;
        using base_type = timer_base;
        using player_timer_component_shared_ptr = std::shared_ptr<player_time_component>;

        explicit player_timer(io_context_type& io_context, duration_type interval, const player_timer_component_shared_ptr& player_timer_component);

    private:
        using player_timer_component_weak_ptr = std::weak_ptr<player_time_component>;

        void execute_timer_task() override;

        io_context_type& io_context_;
        player_timer_component_weak_ptr player_timer_component_;
    };
}