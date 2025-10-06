#include "websocket_session_write.h"

celeritas::websocket_session_write::websocket_session_write(web_socket_stream_type& web_socket)
    : base_type{}, web_socket_{ web_socket }
{
}

void celeritas::websocket_session_write::write(buffer_guard data)
{
}