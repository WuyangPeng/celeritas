#pragma once

#include <boost/asio.hpp>

namespace celeritas
{
    class listener
    {
    public:
        using class_type = listener;

        // 构造函数：接受 io_context 和监听端口
        listener(boost::asio::io_context& io_context, unsigned short port);

        // 开始监听新连接
        void start();

    private:
        using awaitable_type = boost::asio::awaitable<void>;
        using io_context_type = boost::asio::io_context;
        using acceptor_type = boost::asio::ip::tcp::acceptor;

        // 协程：异步接受新连接
        [[nodiscard]] awaitable_type accept_connections();

        io_context_type& io_context_;
        acceptor_type acceptor_;
    };
}


