#pragma once

#include "service_registry_internal_fwd.h"
#include "common/framework/timer_base.h"

namespace celeritas
{
    class cleanup_timer final : public timer_base
    {
    public:
        using class_type = cleanup_timer;
        using base_type = timer_base;
        using service_registry_impl_weak_ptr = std::weak_ptr<service_registry_impl>;

        cleanup_timer(const any_io_executor& any_io_executor, duration_type interval, service_registry_impl_weak_ptr service_registry);

    private:
        void execute_timer_task() override;

        service_registry_impl_weak_ptr service_registry_;
    };
}