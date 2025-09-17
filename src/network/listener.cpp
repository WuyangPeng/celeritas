#include "listener.h"
#include "session.h"

#include <iostream>

// 构造函数
celeritas::listener::listener(boost::asio::io_context& io_context, unsigned short port)
    : io_context_(io_context), acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    // 打印监听地址和端口
    std::cout << "Listening on port " << port << "..." << std::endl;
}

// 启动协程来接受连接
void celeritas::listener::Start()
{
    co_spawn(io_context_,
             [this]
             {
                 return AcceptConnections();
             },
             boost::asio::detached);
}

// 协程：接受连接
boost::asio::awaitable<void> celeritas::listener::AcceptConnections()
{
    try
    {
        while (true)
        {
            // 等待新连接
            boost::asio::ip::tcp::socket socket = co_await acceptor_.async_accept(boost::asio::use_awaitable);
            std::cout << "Accepted new connection from: " << socket.remote_endpoint() << std::endl;

            // 为新连接创建一个会话，并启动
            std::make_shared<session>(std::move(socket))->Start();
        }
    }
    catch (const boost::system::system_error& e)
    {
        std::cerr << "Listener error: " << e.what() << std::endl;
    }
}
