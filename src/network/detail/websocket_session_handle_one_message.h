#pragma once

#include "network/network_message_callback.h"
#include "network/session_callback.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace celeritas
{
    class websocket_session_handle_one_message
    {
    public:
        using class_type = websocket_session_handle_one_message;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using web_socket_stream_type = boost::beast::websocket::stream<boost::beast::tcp_stream>;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;

        websocket_session_handle_one_message(web_socket_stream_type& web_socket, int64_t session_id, session_callback callback);

        [[nodiscard]] void_awaitable_type run();

    private:
        web_socket_stream_type& web_socket_;
        int64_t session_id_;
        session_callback callback_;
    };
}