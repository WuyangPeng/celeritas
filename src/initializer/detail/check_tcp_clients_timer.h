#pragma once

#include "common/timer_base.h"
#include "initializer/initializer_fwd.h"

namespace celeritas
{
    class check_tcp_clients_timer final : public timer_base
    {
    public:
        using class_type = check_tcp_clients_timer;
        using base_type = timer_base;
        using resource_loader_weak_ptr = std::weak_ptr<resource_loader>;

        explicit check_tcp_clients_timer(io_context_type& io_context, duration_type interval, resource_loader_weak_ptr resource_loader);

    private:
        void execute_timer_task() override;

        resource_loader_weak_ptr resource_loader_;
        io_context_type& io_context_;
    };
}

