#include "http_response_session_run.h"

celeritas::http_response_session_run::http_response_session_run(socket_type& socket, const int64_t session_id, session_callback session_callback, std::string path)
    : base_type{ socket, session_id, std::move(session_callback) }, path_{ std::move(path) }
{
}

celeritas::session_run::void_awaitable_type celeritas::http_response_session_run::handle_one_message()
{
    co_await handle_one_response_message(path_);
}