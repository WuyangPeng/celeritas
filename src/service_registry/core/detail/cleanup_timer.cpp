#include "cleanup_timer.h"
#include "service_registry_impl.h"

celeritas::cleanup_timer::cleanup_timer(const any_io_executor& any_io_executor, const duration_type interval, service_registry_impl_weak_ptr service_registry)
    : base_type{ any_io_executor, interval }, service_registry_{ std::move(service_registry) }
{
}

void celeritas::cleanup_timer::execute_timer_task()
{
    if (const auto service_registry_shared_ptr = service_registry_.lock();
        service_registry_shared_ptr != nullptr)
    {
        service_registry_shared_ptr->cleanup_services_by_duration();
    }
}