#include "service_registry_timer.h"
#include "initializer/resource_loader.h"

celeritas::service_registry_timer::service_registry_timer(io_context_type& io_context, const duration_type interval, resource_loader_weak_ptr resource_loader)
    : base_type{ io_context, interval }, resource_loader_{ std::move(resource_loader) }
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