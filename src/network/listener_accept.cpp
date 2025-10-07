#include "listener_accept.h"
#include "session.h"

void celeritas::listener_accept::remove_session(int64_t session_id)
{
    sessions_.erase(session_id);
}

void celeritas::listener_accept::set_stop()
{
    is_running_ = false;
}

bool celeritas::listener_accept::is_running() const noexcept
{
    return is_running_;
}

int64_t celeritas::listener_accept::get_next_session_id() noexcept
{
    return ++session_id_;
}

void celeritas::listener_accept::add_session(const session_shared_ptr& session)
{
    sessions_[session->get_session_id()] = session;
}