#pragma once

#include <chrono>

namespace celeritas
{
    constexpr auto max_header_message_size = 0xFF;
    constexpr auto max_message_size = 16 * 1024 * 1024; // 16 MB
    constexpr auto timeout_seconds = std::chrono::seconds(60);

    class websocket_session_handle_session;
    class websocket_session_handle_one_message;

    template <typename SocketType>
    class tcp_session_write;
}