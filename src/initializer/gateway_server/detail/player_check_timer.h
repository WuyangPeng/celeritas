#pragma once

#include "common/framework/timer_base.h"
#include "initializer/initializer_fwd.h"
#include "network/network_fwd.h"

namespace celeritas
{
    class player_check_timer final : public timer_base
    {
    public:
        using class_type = player_check_timer;
        using base_type = timer_base;
        using resource_loader_weak_ptr = std::weak_ptr<gateway_resource_loader>;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;

        explicit player_check_timer(const any_io_executor& any_io_executor, duration_type interval, resource_loader_weak_ptr resource_loader, network_message_callback_weak_ptr network_message_callback);

    private:
        void execute_timer_task() override;

        any_io_executor any_io_executor_;
        resource_loader_weak_ptr resource_loader_;
        network_message_callback_weak_ptr network_message_callback_;
    };
}

