#pragma once

#include <boost/asio.hpp>

namespace celeritas
{
    class session : public std::enable_shared_from_this<session>
    {
    public:
        using class_type = session;
        using socket_type = boost::asio::ip::tcp::socket;

        // 构造函数：接受一个已连接的 socket
        explicit session(socket_type socket);

        // 启动会话处理协程
        void start();

    private:
        using awaitable_type = boost::asio::awaitable<void>;

        // 协程：处理会话的读写循环
        awaitable_type handle_session();

        socket_type socket_;
    };
}
