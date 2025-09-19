#include "common/logger.h"
#include "ipc_session.h"
#include "message_header.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

celeritas::ipc_session::ipc_session(boost::asio::local::stream_protocol::socket socket)
    : socket_{ std::move(socket) }
{
}

void celeritas::ipc_session::start()
{
    co_spawn(socket_.get_executor(), [self = shared_from_this()] {
        return self->handle_read();
    },
             boost::asio::detached);
}

boost::asio::awaitable<void> celeritas::ipc_session::write(const std::vector<char>& data)
{
    // TODO: 这里需要添加线程安全机制，比如一个发送队列
    co_await boost::asio::async_write(socket_,
                                      boost::asio::buffer(data),
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
            const auto size = header.total_size();
            if (size == 0)
            {
                continue;
            }

            std::vector<char> data(size);
            co_await async_read(socket_,
                                boost::asio::buffer(data, size),
                                boost::asio::use_awaitable);

            LOG(debug) << "Received IPC message of size: "
                       << header.get_header_type()
                       << ",header size:"
                       << header.get_header_size()
                       << ",body size:"
                       << header.get_body_size();
            ;
        }
    }
    catch (const boost::system::system_error& error)
    {
        LOG(warning) << "IPC Session error: " << error.what();
    }
}