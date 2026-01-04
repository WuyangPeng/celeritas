#pragma once

#include "common/framework/resource_loader_base.h"
#include "common/framework/timer_base.h"

namespace celeritas
{
    class check_tcp_clients_timer final : public timer_base
    {
    public:
        using class_type = check_tcp_clients_timer;
        using base_type = timer_base;
        using resource_loader_weak_ptr = std::weak_ptr<resource_loader_base>;

        explicit check_tcp_clients_timer(const any_io_executor& any_io_executor, duration_type interval, resource_loader_weak_ptr resource_loader);

    private:
        void execute_timer_task() override;

        resource_loader_weak_ptr resource_loader_;
        any_io_executor any_io_executor_;
    };
}

