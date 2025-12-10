#include "http_request_session_run.h"

celeritas::http_request_session_run::http_request_session_run(socket_type& socket, const int64_t session_id, session_callback session_callback)
    : base_type{ socket, session_id, std::move(session_callback) }
{
}

celeritas::session_run::void_awaitable_type celeritas::http_request_session_run::handle_one_message()
{
    co_await handle_one_request_message();
}