#pragma once

#include "common/common_fwd.h"
#include "common/framework/timer_base.h"
#include "initializer/initializer_fwd.h"

namespace celeritas
{
    class service_registry_timer final : public timer_base
    {
    public:
        using class_type = service_registry_timer;
        using base_type = timer_base;
        using resource_loader_weak_ptr = std::weak_ptr<resource_loader_base>;

        explicit service_registry_timer(const any_io_executor& any_io_executor, duration_type interval, resource_loader_weak_ptr resource_loader);

    private:
        void execute_timer_task() override;

        resource_loader_weak_ptr resource_loader_;
    };
}

