#pragma once

#include "common/buffer_guard.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace celeritas
{
    namespace beast_websocket = boost::beast::websocket;

    class web_socket_session_write
    {
    public:
        using class_type = web_socket_session_write;
        using tcp_stream_type = boost::beast::tcp_stream;
        using web_socket_stream_type = beast_websocket::stream<tcp_stream_type>;

        explicit web_socket_session_write(web_socket_stream_type& web_socket);

        // 向客户端发送消息
        void write(buffer_guard data);

    private:
        web_socket_stream_type& web_socket_;
    };
}