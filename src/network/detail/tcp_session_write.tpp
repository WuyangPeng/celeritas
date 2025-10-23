#pragma once

#include "tcp_session_write.h"
#include "common/logger.h"

#include <boost/asio.hpp>

template <typename SocketType>
celeritas::tcp_session_write<SocketType>::tcp_session_write(socket_type& socket)
    : base_type{}, socket_{ socket }, write_queue_{}, write_mutex_{}
{
}

template <typename SocketType>
void celeritas::tcp_session_write<SocketType>::write(buffer_guard data)
{
    std::lock_guard lock{ write_mutex_ };
    write_queue_.emplace_back(std::move(data));

    // 如果发送协程没有在运行，就启动它
    if (write_queue_.size() == 1)
    {
        co_spawn(socket_.get_executor(), [self = this->shared_from_this()] {
                     return self->do_write();
                 },
                 boost::asio::detached);
    }
}

template <typename SocketType>
celeritas::session_write::void_awaitable_type celeritas::tcp_session_write<SocketType>::do_write()
{
    while (socket_.is_open())
    {
        try
        {
            if (const auto result = co_await do_one_write();
                !result)
            {
                co_return;
            }
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
        catch (...)
        {
            LOG_CHANNEL(network_channel, fatal) << "Listener unknown error.";
            break;
        }
    }
}

template <typename SocketType>
celeritas::tcp_session_write<SocketType>::bool_awaitable_type celeritas::tcp_session_write<SocketType>::do_one_write()
{
    // 调用新函数来获取数据，该函数内部处理了加锁和解锁
    auto optional_buffer_guard = get_next_write_buffer();
    if (!optional_buffer_guard)
    {
        co_return false; // 队列为空，退出协程
    }
    auto buffer_guard = std::move(*optional_buffer_guard);

    co_await boost::asio::async_write(socket_, boost::asio::buffer(buffer_guard.get(), buffer_guard.get_effective_size()), boost::asio::use_awaitable);
    LOG_CHANNEL(network_channel, debug) << "Successfully wrote " << buffer_guard.get_effective_size() << " bytes to client.";

    co_return true;
}

template <typename SocketType>
celeritas::tcp_session_write<SocketType>::buffer_guard_optional_type celeritas::tcp_session_write<SocketType>::get_next_write_buffer()
{
    std::lock_guard lock{ write_mutex_ };
    if (write_queue_.empty())
    {
        return std::nullopt; // 队列为空，返回一个空对象
    }
    auto buffer = std::move(write_queue_.front());
    write_queue_.pop_front();

    return buffer;
}