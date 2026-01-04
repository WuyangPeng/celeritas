#include "buffer_pool_timer.h"
#include "common/buffer/buffer_pool.h"
#include "service_registry/detail/service_registry_internal_fwd.h"

celeritas::buffer_pool_timer::buffer_pool_timer(const any_io_executor& any_io_executor, const duration_type interval)
    : base_type{ any_io_executor, interval }
{
}

void celeritas::buffer_pool_timer::execute_timer_task()
{
    buffer_pool::reclaim(buffer_pool_reclaim_seconds);
}