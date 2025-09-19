#include "common/buffer_guard.h"
#include "common/buffer_pool.h"
#include "common/logger.h"
#include "ipc_session.h"
#include "message_header.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

celeritas::ipc_session::ipc_session(boost::asio::local::stream_protocol::socket socket, message_handler_type handler)
    : socket_{ std::move(socket) }, message_handler_{ std::move(handler) }
{
}

void celeritas::ipc_session::start()
{
    co_spawn(socket_.get_executor(), [self = shared_from_this()] {
        return self->handle_read();
    },
             boost::asio::detached);
}

boost::asio::awaitable<void> celeritas::ipc_session::write(buffer_pool_data& data)
{
    // TODO: 这里需要添加线程安全机制，比如一个发送队列
    co_await boost::asio::async_write(socket_,
                                      boost::asio::buffer(data.data(), data.size()),
                                      boost::asio::use_awaitable);
}

boost::asio::awaitable<void> celeritas::ipc_session::handle_read()
{
    try
    {
        for (;;)
        {
            message_header header{};
            co_await async_read(socket_,
                                boost::asio::buffer(&header, sizeof(header)),
                                boost::asio::use_awaitable);

            header.network_to_host();
            const auto total_size = header.get_total_size();
            if (total_size == 0)
            {
                continue;
            }

            buffer_guard buffer_guard{ buffer_pool::acquire(total_size) };
            co_await async_read(socket_,
                                boost::asio::buffer(buffer_guard.get(), total_size),
                                boost::asio::use_awaitable);

            LOG(debug) << "Received IPC message of size: "
                       << header.get_header_type()
                       << ",header size:"
                       << header.get_header_size()
                       << ",body size:"
                       << header.get_body_size();

            // 现在，通知外部处理者一个完整的消息已经接收到
            // 我们将消息头和消息体数据传递给回调函数
            if (message_handler_ != nullptr)
            {
                message_handler_(header, std::move(buffer_guard));
            }
        }
    }
    catch (const boost::system::system_error& error)
    {
        LOG(warning) << "IPC Session error: " << error.what();
    }
}