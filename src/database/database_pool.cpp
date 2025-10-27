#include "database_pool.h"

void celeritas::database_pool::start_cleanup_timer(io_context_type& io_context)
{
    cleanup_database_session_timer_ = std::make_unique<cleanup_database_session_timer>(io_context, cleanup_database_timer, shared_from_this());

    cleanup_database_session_timer_->start();
}

void celeritas::database_pool::stop_cleanup_timer()
{
    if (cleanup_database_session_timer_ != nullptr)
    {
        cleanup_database_session_timer_->stop();
    }
}