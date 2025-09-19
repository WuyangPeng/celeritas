#pragma once

#include "common/common_fwd.h"
#include "network_fwd.h"

#include <boost/asio.hpp>

namespace celeritas
{
    class listener
    {
    public:
        using class_type = listener;
        using message_handler_type = std::function<void(const message_header&, buffer_guard)>;

        // 构造函数：接受 io_context 和监听端口
        listener(boost::asio::io_context& io_context, uint16_t port, message_handler_type handler);

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

        io_context_type& io_context_;
        acceptor_type acceptor_;
        message_handler_type message_handler_;
        std::atomic<bool> is_running_;
    };
}
