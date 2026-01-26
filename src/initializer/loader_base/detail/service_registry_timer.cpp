#include "service_registry_timer.h"
#include "initializer/loader_base/resource_loader.h"

celeritas::service_registry_timer::service_registry_timer(const any_io_executor& any_io_executor, const duration_type interval, resource_loader_weak_ptr resource_loader)
    : base_type{ any_io_executor, interval }, resource_loader_{ std::move(resource_loader) }
{
}

void celeritas::service_registry_timer::execute_timer_task()
{
    if (const auto resource_loader_shared_ptr = resource_loader_.lock();
        resource_loader_shared_ptr != nullptr)
    {
        resource_loader_shared_ptr->process_service_registry_by_duration();
    }
}