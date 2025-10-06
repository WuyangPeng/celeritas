#pragma once

#include "common/buffer_guard.h"
#include "network/session_write.h"

#include <boost/beast.hpp>

namespace celeritas
{
    namespace beast_websocket = boost::beast::websocket;

    class websocket_session_write : public session_write
    {
    public:
        using class_type = websocket_session_write;
        using base_type = session_write;
        using tcp_stream_type = boost::beast::tcp_stream;
        using web_socket_stream_type = beast_websocket::stream<tcp_stream_type>;

        explicit websocket_session_write(web_socket_stream_type& web_socket);

        // 向客户端发送消息
        void write(buffer_guard data) override;

        [[nodiscard]] void_awaitable_type do_write() override;

    private:
        web_socket_stream_type& web_socket_;
    };
}