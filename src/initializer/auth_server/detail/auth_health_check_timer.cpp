#include "auth_health_check_timer.h"
#include "initializer/auth_server/auth_resource_loader.h"

celeritas::auth_health_check_timer::auth_health_check_timer(const any_io_executor& any_io_executor, const duration_type interval, resource_loader_weak_ptr resource_loader, network_message_callback_weak_ptr network_message_callback)
    : base_type{ any_io_executor, interval }, any_io_executor_{ any_io_executor }, resource_loader_{ std::move(resource_loader) }, network_message_callback_{ std::move(network_message_callback) }
{
}

void celeritas::auth_health_check_timer::execute_timer_task()
{
    if (const auto resource_loader_shared_ptr = resource_loader_.lock();
        resource_loader_shared_ptr != nullptr)
    {
        resource_loader_shared_ptr->send_health_check(any_io_executor_, network_message_callback_);
    }
}