#include "session.h"

celeritas::session::session(int64_t session_id) noexcept
    : session_id_{ session_id }
{
}

int64_t celeritas::session::get_session_id() const noexcept
{
    return session_id_;
}