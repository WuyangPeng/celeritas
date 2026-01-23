#include "mock_listener_sessions.h"

void celeritas::mock_listener_sessions::remove_session(const int64_t session_id)
{
    remove_session_called_ = true;
    last_removed_session_id_ = session_id;
}

bool celeritas::mock_listener_sessions::is_remove_session_called() const
{
    return remove_session_called_;
}

int64_t celeritas::mock_listener_sessions::get_last_removed_session_id() const
{
    return last_removed_session_id_;
}
