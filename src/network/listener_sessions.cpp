#include "listener_sessions.h"
#include "session_base.h"
#include "common/celeritas_error.h"

void celeritas::listener_sessions::remove_session(const int64_t session_id)
{
    if (const auto iter = sessions_.find(session_id);
        iter != sessions_.cend())
    {
        iter->second->stop();
        sessions_.erase(iter);
    }
}

void celeritas::listener_sessions::set_stop()
{
    is_running_ = false;
}

bool celeritas::listener_sessions::is_running() const noexcept
{
    return is_running_;
}

int64_t celeritas::listener_sessions::get_next_session_id() noexcept
{
    return ++session_id_;
}

void celeritas::listener_sessions::add_session(const session_shared_ptr& session)
{
    sessions_[session->get_session_id()] = session;
}

celeritas::listener_sessions::session_shared_ptr celeritas::listener_sessions::get_session(const int64_t id)
{
    const auto iter = sessions_.find(id);
    if (iter == sessions_.cend())
    {
        throw celeritas_error("no session found for id {}", id);
    }

    return iter->second;
}