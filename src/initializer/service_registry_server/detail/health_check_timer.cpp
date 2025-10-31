#include "health_check_timer.h"

celeritas::health_check_timer::health_check_timer(io_context_type& io_context, duration_type interval, resource_loader_weak_ptr resource_loader)
    : base_type{ io_context, interval }, resource_loader_{ resource_loader }
{
}

void celeritas::health_check_timer::execute_timer_task()
{
}