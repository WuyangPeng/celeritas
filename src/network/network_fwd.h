#pragma once

namespace celeritas
{
    template <typename SocketType>
    class session_base;

    class session;
    class listener;
    class tcp_listener;
    class tcp_client;
    class message_header;
    class network_message_callback;
    class http_session;
    class http_listener;
    class websocket_session;
    class websocket_listener;
    class session_callback;
    class session_write;
    class listener_accept;
}