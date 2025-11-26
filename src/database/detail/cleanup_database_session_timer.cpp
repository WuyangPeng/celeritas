#include "cleanup_database_session_timer.h"
#include "database/database_pool_base.h"

celeritas::cleanup_database_session_timer::cleanup_database_session_timer(io_context_type& io_context, const duration_type interval, database_pool_weak_ptr database_pool)
    : base_type{ io_context, interval }, database_pool_{ std::move(database_pool) }
{
}

void celeritas::cleanup_database_session_timer::execute_timer_task()
{
    if (const auto database_pool_shared_ptr = database_pool_.lock();
        database_pool_shared_ptr != nullptr)
    {
        database_pool_shared_ptr->cleanup_database_by_duration();
    }
}