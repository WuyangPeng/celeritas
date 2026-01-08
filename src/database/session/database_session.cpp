#include "database_session.h"
#include "database/database_constant.h"

bool celeritas::database_session::is_expired() const
{
    return std::chrono::steady_clock::now() - last_heartbeat_ > cleanup_database_expired_timer;
}

void celeritas::database_session::set_last_heartbeat()
{
    last_heartbeat_ = std::chrono::steady_clock::now();
}