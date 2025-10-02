#pragma once

#include "listener.h"
#include "network_message_callback.h"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <deque>
#include <memory>

namespace celeritas
{
    class websocket_session : public std::enable_shared_from_this<websocket_session>
    {
    public:
        using class_type = websocket_session;
        using socket_type = boost::asio::ip::tcp::socket;
        using listener_weak_ptr = std::weak_ptr<listener>;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;

        // WebSocket 流的类型
        using web_socket_stream_type = boost::beast::websocket::stream<boost::beast::tcp_stream>;

        explicit websocket_session(socket_type socket,
                                   long session_id,
                                   network_message_callback_weak_ptr network_message_callback,
                                   const std::string& game_server_id,
                                   listener_weak_ptr listener);

        // 启动会话处理协程
        void start();

    private:
        using void_awaitable_type = boost::asio::awaitable<void>;

        // 协程：处理会话的读写循环
        [[nodiscard]] void_awaitable_type run();

        void set_option(const std::string& game_server_id);

        void close_web_socket();

        web_socket_stream_type web_socket_;
        long session_id_;
        network_message_callback_weak_ptr network_message_callback_;
        listener_weak_ptr listener_;
    };
}

