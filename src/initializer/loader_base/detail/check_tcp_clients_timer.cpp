#include "check_tcp_clients_timer.h"
#include "initializer/loader_base/resource_loader.h"

celeritas::check_tcp_clients_timer::check_tcp_clients_timer(const any_io_executor& any_io_executor, const duration_type interval, resource_loader_weak_ptr resource_loader)
    : base_type{ any_io_executor, interval }, resource_loader_{ std::move(resource_loader) }, any_io_executor_{ any_io_executor }
{
}

void celeritas::check_tcp_clients_timer::execute_timer_task()
{
    if (const auto resource_loader_shared_ptr = resource_loader_.lock();
        resource_loader_shared_ptr != nullptr)
    {
        resource_loader_shared_ptr->process_check_tcp_clients_by_duration(any_io_executor_);
    }
}