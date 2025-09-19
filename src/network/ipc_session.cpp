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

void celeritas::ipc_session::write(buffer_guard data)
{
    std::lock_guard lock{ write_mutex_ };

    write_queue_.emplace_back(std::move(data));

    // 如果发送协程没有在运行，就启动它
    if (write_queue_.size() == 1)
    {
        co_spawn(socket_.get_executor(), [self = shared_from_this()] {
            return self->do_write();
        },
                 boost::asio::detached);
    }
}

boost::asio::awaitable<void> celeritas::ipc_session::do_write()
{
    // 持续处理发送队列直到为空
    while (socket_.is_open())
    {
        try
        {
            co_await do_one_write();
        }
        catch (const boost::system::system_error& error)
        {
            LOG_CHANNEL(network_channel, warning) << "Write error: " << error.what();
            break;
        }
        catch (const std::exception& error)
        {
            LOG_CHANNEL(network_channel, error) << "Write unknown error: " << error.what();
            break;
        }
    }
}

boost::asio::awaitable<void> celeritas::ipc_session::do_one_write()
{
    auto optional_buffer_guard = get_next_write_buffer();
    if (!optional_buffer_guard)
    {
        co_return;  // 队列为空，退出协程
    }
    auto buffer_guard = std::move(*optional_buffer_guard);

    co_await boost::asio::async_write(socket_, boost::asio::buffer(buffer_guard.get(), buffer_guard.get_effective_size()), boost::asio::use_awaitable);
    LOG_CHANNEL(network_channel, debug) << "Successfully wrote " << buffer_guard.get_effective_size() << " bytes.";
}

celeritas::ipc_session::buffer_guard_optional_type celeritas::ipc_session::get_next_write_buffer()
{
    std::lock_guard lock{ write_mutex_ };
    if (write_queue_.empty())
    {
        return std::nullopt;  // 队列为空，返回一个空对象
    }
    auto buffer = std::move(write_queue_.front());
    write_queue_.pop_front();

    return buffer;
}

boost::asio::awaitable<void> celeritas::ipc_session::handle_read()
{
    while (socket_.is_open())
    {
        try
        {
            co_await handle_one_message();
        }
        catch (const boost::system::system_error& error)
        {
            LOG_CHANNEL(network_channel, warning) << "IPC Session error: " << error.what();
        }
        catch (...)
        {
            LOG_CHANNEL(network_channel, fatal) << "Listener unknown error.";
        }
    }
}

celeritas::ipc_session::awaitable_type celeritas::ipc_session::handle_one_message()
{
    message_header header{};
    co_await async_read(socket_,
                        boost::asio::buffer(&header, sizeof(header)),
                        boost::asio::use_awaitable);

    header.network_to_host();
    const auto total_size = header.get_total_size();
    if (total_size == 0)
    {
        co_return;
    }

    buffer_guard buffer_guard{ buffer_pool::acquire(total_size) };
    co_await async_read(socket_,
                        boost::asio::buffer(buffer_guard.get(), total_size),
                        boost::asio::use_awaitable);

    LOG_CHANNEL(network_channel, debug) << "Received IPC message of size: "
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