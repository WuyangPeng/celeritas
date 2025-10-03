#include "session.h"

celeritas::session::session(int64_t session_id, session_callback session_callback)
    : session_id_{ session_id }, session_callback_{ std::move(session_callback) }
{
}

int64_t celeritas::session::get_session_id() const noexcept
{
    return session_id_;
}

void celeritas::session::remove_session()
{
    session_callback_.remove_session(session_id_);
}

celeritas::session::network_message_callback_weak_ptr celeritas::session::get_network_message_callback()
{
    return session_callback_.get_network_message_callback();
}