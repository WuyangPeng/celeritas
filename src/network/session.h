#pragma once

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <memory>
#include <vector>
#include <queue>
#include <mutex>

namespace celeritas
{
    // 假设我们有一个消息头来指示消息大小
    struct message_header
    {
        uint32_t size;
    };

    class session : public std::enable_shared_from_this<session>
    {
    public:
        // 构造函数：接受一个已连接的 socket
        explicit session(boost::asio::ip::tcp::socket socket);

        // 启动会话处理协程
        void Start();

    private:
        // 协程：处理会话的读写循环
        boost::asio::awaitable<void> HandleSession();

        boost::asio::ip::tcp::socket socket_;
    };
}
