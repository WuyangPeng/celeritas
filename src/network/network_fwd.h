#pragma once

namespace celeritas
{
    template <typename SocketType>
    class session_base;

    class session;
    class websocket_session;
    class http_session;
    class listener;
    class listener_accept;
    class websocket_listener;
    class tcp_listener;
    class http_listener;
    class tcp_client;
    class session_write;
    class session_run;
    class session_callback;
    class network_message_callback;
    class message_header;
    class listener_sessions;
    class listener_sessions_base;
}