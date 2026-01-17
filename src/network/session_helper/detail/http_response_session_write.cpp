#include "network/session_helper/detail/http_response_session_write.h"

celeritas::http_response_session_write::http_response_session_write(socket_type& socket)
    : base_type{ socket, "" }
{
}

celeritas::http_response_session_write::bool_awaitable_type celeritas::http_response_session_write::do_one_write()
{
    co_return co_await do_one_write_response();
}