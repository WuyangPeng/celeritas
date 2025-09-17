#include "message_header.h"
#include "session.h"
#include "common/logger.h"

celeritas::session::session(boost::asio::ip::tcp::socket socket)
    : socket_{ std::move(socket) }
{
}

// 启动协程来处理会话
void celeritas::session::start()
{
    co_spawn(socket_.get_executor(),
             [self = shared_from_this()] {
                 return self->handle_session();
             },
             boost::asio::detached);
}

// 协程：处理会话读写
boost::asio::awaitable<void> celeritas::session::handle_session()
{
    try
    {
        while (true)
        {
            // 1. 读取消息头
            message_header header{};
            co_await async_read(socket_,
                                boost::asio::buffer(&header, sizeof(header)),
                                boost::asio::use_awaitable);

            // 转换字节序
            header.size = ntohl(header.size);
            if (header.size == 0)
            {
                continue;
            }

            // 2. 读取消息体
            std::vector<char> data(header.size);
            co_await async_read(socket_,
                                boost::asio::buffer(data, header.size),
                                boost::asio::use_awaitable);

            LOG(debug) << "Received message of size: " << header.size;
        }
    }
    catch (const boost::system::system_error& error)
    {
        if (error.code() != boost::asio::error::eof &&
            error.code() != boost::asio::error::connection_reset)
        {
            LOG(warning) << "Session error: " << error.what();
        }
    }
}
