#pragma once

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/ts/internet.hpp>
#include <memory>

namespace celeritas
{
    class session : public std::enable_shared_from_this<session>
    {
    public:
        using class_type = session;

        // 构造函数：接受一个已连接的 socket
        explicit session(boost::asio::ip::tcp::socket socket);

        // 启动会话处理协程
        void start();

    private:
        // 协程：处理会话的读写循环
        boost::asio::awaitable<void> handle_session();

        boost::asio::ip::tcp::socket socket_;
    };
}
