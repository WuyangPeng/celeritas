#pragma once

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>

namespace celeritas
{
    class listener
    {
    public:
        // 构造函数：接受 io_context 和监听端口
        listener(boost::asio::io_context& io_context, unsigned short port);

        // 开始监听新连接
        void Start();

    private:
        // 协程：异步接受新连接
        boost::asio::awaitable<void> AcceptConnections();

        boost::asio::io_context& io_context_;
        boost::asio::ip::tcp::acceptor acceptor_;
    };
}


