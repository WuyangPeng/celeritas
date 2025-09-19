#pragma once

#include "common/common_fwd.h"
#include "network_fwd.h"

#include <boost/asio.hpp>

namespace celeritas
{
    class tcp_listener
    {
    public:
        using class_type = tcp_listener;
        using session_type = session_base<boost::asio::ip::tcp::socket>;
        using message_handler_type = session_type::message_handler_type;

        // 构造函数：接受 io_context 和监听端口
        tcp_listener(boost::asio::io_context& io_context, uint16_t port, message_handler_type handler);

        // 开始监听新连接
        void start();

        // 停止监听器
        void stop();

    private:
        using awaitable_type = boost::asio::awaitable<void>;
        using io_context_type = boost::asio::io_context;
        using acceptor_type = boost::asio::ip::tcp::acceptor;

        // 协程：异步接受新连接
        [[nodiscard]] awaitable_type accept_connections();
        [[nodiscard]] awaitable_type handle_connection();

        io_context_type& io_context_;
        acceptor_type acceptor_;
        message_handler_type message_handler_;
        std::atomic<bool> is_running_;
    };
}
