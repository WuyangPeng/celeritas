#pragma once

#include "common/framework/timer_base.h"
#include "player/player_fwd.h"

namespace celeritas
{
    class player_timer final : public timer_base
    {
    public:
        using class_type = player_timer;
        using base_type = timer_base;
        using player_timer_component_shared_ptr = std::shared_ptr<player_time_component>;

        explicit player_timer(const any_io_executor& any_io_executor, duration_type interval, const player_timer_component_shared_ptr& player_timer_component);

    private:
        using player_timer_component_weak_ptr = std::weak_ptr<player_time_component>;

        void execute_timer_task() override;

        any_io_executor any_io_executor_;
        player_timer_component_weak_ptr player_timer_component_;
    };
}