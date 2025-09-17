#include "session.h"

#include <iostream>

// 构造函数
celeritas::session::session(boost::asio::ip::tcp::socket socket)
    : socket_(std::move(socket))
{
}

// 启动协程来处理会话
void celeritas::session::Start()
{
    co_spawn(socket_.get_executor(),
             [self = shared_from_this()]
             {
                 return self->HandleSession();
             },
             boost::asio::detached);
}

// 协程：处理会话读写
boost::asio::awaitable<void> celeritas::session::HandleSession()
{
    try
    {
        while (true)
        {
            // 1. 读取消息头
            message_header header;
            co_await async_read(socket_,
                                boost::asio::buffer(&header, sizeof(header)),
                                boost::asio::use_awaitable);

            // 转换字节序
            header.size = ntohl(header.size);
            if (header.size == 0) continue;

            // 2. 读取消息体
            std::vector<char> data(header.size);
            co_await async_read(socket_,
                                boost::asio::buffer(data, header.size),
                                boost::asio::use_awaitable);

            std::cout << "Received message of size: " << header.size << std::endl;
            // TODO: 在这里处理接收到的消息
            // ...
        }
    }
    catch (const boost::system::system_error& e)
    {
        if (e.code() != boost::asio::error::eof &&
            e.code() != boost::asio::error::connection_reset)
        {
            std::cerr << "Session error: " << e.what() << std::endl;
        }
    }
}
