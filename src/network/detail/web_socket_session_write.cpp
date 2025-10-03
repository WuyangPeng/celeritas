#include "web_socket_session_write.h"

celeritas::web_socket_session_write::web_socket_session_write(web_socket_stream_type& web_socket)
    : web_socket_{ web_socket }
{
}

void celeritas::web_socket_session_write::write(buffer_guard data)
{
}