#include "database_pool_base.h"

void celeritas::database_pool_base::start_cleanup_timer(const any_io_executor& any_io_executor)
{
    cleanup_database_session_timer_ = std::make_shared<cleanup_database_session_timer>(any_io_executor, cleanup_database_timer, shared_from_this());

    cleanup_database_session_timer_->start();
}

void celeritas::database_pool_base::stop_cleanup_timer()
{
    if (cleanup_database_session_timer_ != nullptr)
    {
        cleanup_database_session_timer_->stop();
    }
}

celeritas::database_pool_base::bool_awaitable_type celeritas::database_pool_base::execute_changes(const database_entity_change_const_shared_ptr& database)
{
    co_return co_await execute_changes(database, 0);
}