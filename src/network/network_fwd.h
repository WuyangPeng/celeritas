#pragma once

namespace celeritas
{
    class session_base;
    class listener;
    class network_message_callback;
    class message_header;
    class http_parameter;

    template <typename SocketType>
    class generic_session;

    class http_session;
    class websocket_session;

    class session_run;
    class session_write;
    class session_callback;

    class listener_accept;
    class listener_sessions_base;
    class listener_sessions;
    class tcp_listener;
    class http_listener;
    class websocket_listener;

    class tcp_client;
    class http_client;
}