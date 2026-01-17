#pragma once

namespace celeritas
{
    class buffer_consumer;
    class read_buffer_guard;

    template <typename SocketType>
    class tcp_session_write;

    template <typename SocketType>
    class tcp_session_run;

    class websocket_session_handle_session;
    class websocket_session_handle_one_message;
    class websocket_session_write;
    class websocket_session_run;

    class http_session_write;
    class http_session_run;
    class http_response_session_write;
    class http_request_session_run;
    class http_response_session_run;
    class http_request_session_write;
}