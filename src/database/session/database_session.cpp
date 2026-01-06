#include "database/database_fwd.h"
#include "database_session.h"

bool celeritas::database_session::is_expired() const
{
    return std::chrono::steady_clock::now() - last_heartbeat > cleanup_database_expired_timer;
}

void celeritas::database_session::set_last_heartbeat()
{
    last_heartbeat = std::chrono::steady_clock::now();
}