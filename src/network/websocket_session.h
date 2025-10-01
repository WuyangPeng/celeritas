#pragma once

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
        using network_message_callback_shared_ptr = std::shared_ptr<network_message_callback>;

        // WebSocket 流的类型
        using ws_stream_type = boost::beast::websocket::stream<boost::beast::tcp_stream>;

        // 构造函数：接受一个已连接的 socket
        explicit websocket_session(boost::asio::ip::tcp::socket socket,
                                   long session_id,
                                   const network_message_callback_shared_ptr& callback);

        // 启动会话处理协程
        void start();

        // 向客户端发送消息（WebSocket 独有的接口）
        void write(std::string message);

    private:
        using void_awaitable_type = boost::asio::awaitable<void>;

        // 协程：处理会话的读写循环
        [[nodiscard]] void_awaitable_type run();

        // 协程：处理发送队列
        [[nodiscard]] void_awaitable_type do_write();

        ws_stream_type ws_;
        long session_id_;
        network_message_callback_shared_ptr network_message_callback_;

        // 写入队列
        std::deque<std::string> write_queue_;
        // 保护写入队列，因为写操作可能由外部（如 write()）和内部（如 do_write()）触发
        std::mutex write_mutex_;
    };
}

