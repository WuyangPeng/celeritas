#pragma once

#include "message_header.h"
#include "session_base.h"
#include "common/buffer_guard.h"
#include "common/buffer_pool.h"
#include "common/logger.h"
#include "detail/network_internal_fwd.h"

template <typename SocketType>
celeritas::session_base<SocketType>::session_base(socket_type socket, message_handler_type handler)
    : socket_{ std::move(socket) }, message_handler_{ std::move(handler) }
{
}

template <typename SocketType>
void celeritas::session_base<SocketType>::start()
{
    co_spawn(socket_.get_executor(), [self = this->shared_from_this()] {
                 return self->handle_session();
             },
             boost::asio::detached);
}

template <typename SocketType>
celeritas::session_base<SocketType>::void_awaitable_type celeritas::session_base<SocketType>::handle_session()
{
    while (socket_.is_open())
    {
        try
        {
            co_await handle_one_message();
        }
        catch (const boost::system::system_error& error)
        {
            if (error.code() == boost::asio::error::eof ||
                error.code() == boost::asio::error::connection_reset)
            {
                LOG_CHANNEL(network_channel, info) << "Session closed gracefully.";
            }
            else
            {
                LOG_CHANNEL(network_channel, warning) << "Session error: " << error.what();
            }
            socket_.close();
            break;
        }
        catch (const std::exception& error)
        {
            LOG_CHANNEL(network_channel, error) << "An unexpected error occurred: " << error.what();
            socket_.close();
            break;
        }
        catch (...)
        {
            LOG_CHANNEL(network_channel, fatal) << "Listener unknown error.";
            socket_.close();
            break;
        }
    }
}

template <typename SocketType>
celeritas::session_base<SocketType>::read_awaitable_type celeritas::session_base<SocketType>::read_data_with_timeout(boost::asio::mutable_buffer buffer)
{
    boost::asio::steady_timer timer{ socket_.get_executor(), std::chrono::steady_clock::now() + timeout_seconds };
    boost::asio::cancellation_signal cancel_signal{};

    co_spawn(socket_.get_executor(), [&]() -> boost::asio::awaitable<void> {
                 auto await_token = boost::asio::as_tuple(boost::asio::use_awaitable);
                 if (auto [error_code] = co_await timer.async_wait(await_token);
                     error_code != boost::asio::error::operation_aborted)
                 {
                     cancel_signal.emit(boost::asio::cancellation_type::all);
                 }
                 co_return;
             },
             boost::asio::detached);

    auto await_token = boost::asio::as_tuple(boost::asio::bind_cancellation_slot(cancel_signal.slot(), boost::asio::use_awaitable));

    auto [read_error_code, bytes_read] =
        co_await boost::asio::async_read(socket_, buffer, await_token);

    timer.cancel();

    if (read_error_code)
    {
        if (read_error_code == boost::asio::error::operation_aborted)
        {
            throw boost::system::system_error(boost::asio::error::timed_out, "Read timed out");
        }
        throw boost::system::system_error(read_error_code, "Failed to read data");
    }

    co_return bytes_read;
}

template <typename SocketType>
celeritas::session_base<SocketType>::void_awaitable_type celeritas::session_base<SocketType>::handle_one_message()
{
    // 读取消息头
    message_header header{};
    co_await read_data_with_timeout(boost::asio::buffer(&header, sizeof(header)));

    // 转换字节序
    header.network_to_host();
    const auto total_size = header.get_total_size();
    if (total_size == 0)
    {
        co_return;
    }

    if (!header.is_effective())
    {
        LOG_CHANNEL(network_channel, error) << "oversized msg, drop connection";
        throw boost::system::system_error(boost::asio::error::message_size);
    }

    buffer_guard buffer_guard{ buffer_pool::acquire(total_size) };
    buffer_guard.set_effective_size(total_size);
    co_await read_data_with_timeout(boost::asio::buffer(buffer_guard.get(), total_size));

    // 日志
    LOG_CHANNEL(network_channel, debug) << "Received message of type: "
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

template <typename SocketType>
void celeritas::session_base<SocketType>::write(buffer_guard data)
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
celeritas::session_base<SocketType>::void_awaitable_type celeritas::session_base<SocketType>::do_write()
{
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
        catch (...)
        {
            LOG_CHANNEL(network_channel, fatal) << "Listener unknown error.";
            break;
        }
    }
}

template <typename SocketType>
celeritas::session_base<SocketType>::void_awaitable_type celeritas::session_base<SocketType>::do_one_write()
{
    // 调用新函数来获取数据，该函数内部处理了加锁和解锁
    auto optional_buffer_guard = get_next_write_buffer();
    if (!optional_buffer_guard)
    {
        co_return; // 队列为空，退出协程
    }
    auto buffer_guard = std::move(*optional_buffer_guard);

    co_await boost::asio::async_write(socket_, boost::asio::buffer(buffer_guard.get(), buffer_guard.get_effective_size()), boost::asio::use_awaitable);
    LOG_CHANNEL(network_channel, debug) << "Successfully wrote " << buffer_guard.get_effective_size() << " bytes to client.";
}

template <typename SocketType>
celeritas::session_base<SocketType>::buffer_guard_optional_type celeritas::session_base<SocketType>::get_next_write_buffer()
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
