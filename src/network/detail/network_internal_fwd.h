#pragma once

#include <chrono>

namespace celeritas
{
    constexpr auto max_header_message_size = 0xFF;
    constexpr auto max_message_size = 16 * 1024 * 1024; // 16 MB
    constexpr auto timeout_seconds = std::chrono::seconds(60);

    class websocket_session_handle_session;
    class websocket_session_handle_one_message;
    class websocket_session_write;
    class websocket_session_run;
    class websocket_listener_accept;
    class tcp_listener_accept;
    class http_session_write;
    class http_session_run;
    class http_listener_accept;
    class write_protobuf_message;
    class buffer_consumer;

    template <typename SocketType>
    class tcp_session_write;

    template <typename SocketType>
    class tcp_session_run;
}