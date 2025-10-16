#include "session_run.h"

void celeritas::session_run::start(const session_weak_ptr& session)
{
    session_ = session;

    do_start();
}

celeritas::session_run::session_shared_ptr celeritas::session_run::get_session()
{
    return session_.lock();
}