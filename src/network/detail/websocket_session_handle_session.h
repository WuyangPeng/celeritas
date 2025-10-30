#pragma once

#include "network/network_fwd.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace celeritas
{
    class websocket_session_handle_session
    {
    public:
        using class_type = websocket_session_handle_session;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using web_socket_stream_type = boost::beast::websocket::stream<boost::beast::tcp_stream>;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;
        using session_weak_ptr = std::weak_ptr<session>;

        websocket_session_handle_session(web_socket_stream_type& web_socket,
                                         int64_t session_id,
                                         network_message_callback_weak_ptr callback,
                                         session_weak_ptr session);

        // 协程：处理会话的读写循环
        [[nodiscard]] void_awaitable_type run();

    private:
        [[nodiscard]] void_awaitable_type handle_one_message();

        web_socket_stream_type& web_socket_;
        int64_t session_id_;
        network_message_callback_weak_ptr callback_;
        session_weak_ptr session_;
    };
}