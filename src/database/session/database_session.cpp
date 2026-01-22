#include "database_session.h"
#include "database/database_constant.h"

#include <boost/asio/strand.hpp>

celeritas::database_session::database_session(const any_io_executor& any_io_executor)
    : last_heartbeat_{ std::chrono::steady_clock::now() }, any_io_executor_{ boost::asio::make_strand(any_io_executor) }
{
}

bool celeritas::database_session::is_expired() const
{
    return std::chrono::steady_clock::now() - last_heartbeat_ > cleanup_database_expired_timer;
}

void celeritas::database_session::set_last_heartbeat()
{
    last_heartbeat_ = std::chrono::steady_clock::now();
}

celeritas::database_session::any_io_executor celeritas::database_session::get_any_io_executor() const
{
    return any_io_executor_;
}