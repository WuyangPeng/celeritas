#pragma once

#include "common/common_fwd.h"
#include "network/network_message_callback.h"

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
        using session_weak_ptr = std::weak_ptr<session>;

        websocket_session_handle_one_message(web_socket_stream_type& web_socket, int64_t session_id, network_message_callback_weak_ptr callback, session_weak_ptr session);

        [[nodiscard]] void_awaitable_type run() const;

    private:
        using flat_buffer_type = boost::beast::flat_buffer;

        [[nodiscard]] void_awaitable_type run(flat_buffer_type& buffer) const;

        [[nodiscard]] void_awaitable_type do_run(flat_buffer_type& buffer) const;

        void call_back(const message_header& message_header, buffer_guard buffer_guard) const;

        web_socket_stream_type& web_socket_;
        int64_t session_id_;
        network_message_callback_weak_ptr callback_;
        session_weak_ptr session_;
    };
}