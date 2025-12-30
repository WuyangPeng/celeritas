#pragma once

#include "common/framework/timer_base.h"
#include "initializer/initializer_fwd.h"
#include "network/network_fwd.h"

namespace celeritas
{
    class auth_health_check_timer final : public timer_base
    {
    public:
        using class_type = auth_health_check_timer;
        using base_type = timer_base;
        using resource_loader_weak_ptr = std::weak_ptr<auth_resource_loader>;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;

        explicit auth_health_check_timer(io_context_type& io_context, duration_type interval, resource_loader_weak_ptr resource_loader, network_message_callback_weak_ptr network_message_callback);

    private:
        void execute_timer_task() override;

        io_context_type& io_context_;
        resource_loader_weak_ptr resource_loader_;
        network_message_callback_weak_ptr network_message_callback_;
    };
}

