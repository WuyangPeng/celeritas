#include "session_run.h"

void celeritas::session_run::start(const session_weak_ptr& session)
{
    session_ = session;

    do_start();
}

celeritas::session_run::void_awaitable_type celeritas::session_run::start_awaitable(const session_weak_ptr& session)
{
    session_ = session;

    co_await run();
}

celeritas::session_run::session_shared_ptr celeritas::session_run::get_session()
{
    return session_.lock();
}

void celeritas::session_run::send_offline_message()
{
}