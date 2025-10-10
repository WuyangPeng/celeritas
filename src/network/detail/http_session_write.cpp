#include "http_session_write.h"

celeritas::http_session_write::http_session_write(socket_type& socket)
    : base_type{}, socket_{ socket }
{
}

void celeritas::http_session_write::write(buffer_guard data)
{
}

celeritas::session_write::void_awaitable_type celeritas::http_session_write::do_write()
{
    co_return;
}