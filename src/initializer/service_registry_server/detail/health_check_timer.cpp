#include "health_check_timer.h"
#include "initializer/service_registry_server/service_registry_resource_loader.h"

celeritas::health_check_timer::health_check_timer(io_context_type& io_context, const duration_type interval, resource_loader_weak_ptr resource_loader, const network_message_callback_weak_ptr& network_message_callback)
    : base_type{ io_context, interval }, io_context_{ io_context }, resource_loader_{ std::move(resource_loader) }, network_message_callback_{ network_message_callback }
{
}

void celeritas::health_check_timer::execute_timer_task()
{
    if (const auto resource_loader_shared_ptr = resource_loader_.lock();
        resource_loader_shared_ptr != nullptr)
    {
        resource_loader_shared_ptr->send_health_check(io_context_, network_message_callback_);
    }
}