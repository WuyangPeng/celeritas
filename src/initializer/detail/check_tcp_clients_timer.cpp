#include "check_tcp_clients_timer.h"
#include "initializer/resource_loader.h"

celeritas::check_tcp_clients_timer::check_tcp_clients_timer(io_context_type& io_context, const duration_type interval, resource_loader_weak_ptr resource_loader)
    : base_type{ io_context, interval }, resource_loader_{ std::move(resource_loader) }, io_context_{ io_context }
{
}

void celeritas::check_tcp_clients_timer::execute_timer_task()
{
    if (const auto resource_loader_shared_ptr = resource_loader_.lock();
        resource_loader_shared_ptr != nullptr)
    {
        resource_loader_shared_ptr->process_check_tcp_clients_by_duration(io_context_);
    }
}