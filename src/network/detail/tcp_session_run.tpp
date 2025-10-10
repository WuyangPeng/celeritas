#pragma once

#include "network_internal_fwd.h"
#include "tcp_session_run.h"
#include "common/buffer_guard.h"
#include "common/buffer_pool.h"
#include "common/common_fwd.h"
#include "common/logger.h"
#include "network/message_header.h"
#include "network/network_message_callback.h"

#include <boost/asio.hpp>

template <typename SocketType>
celeritas::tcp_session_run<SocketType>::tcp_session_run(socket_type& socket, session_callback session_callback)
    : socket_{ socket }, session_callback_{ std::move(session_callback) }
{
}

template <typename SocketType>
void celeritas::tcp_session_run<SocketType>::start()
{
    co_spawn(socket_.get_executor(), [self = this->shared_from_this()] {
                 return self->run();
             },
             boost::asio::detached);
}

template <typename SocketType>
celeritas::session_run::void_awaitable_type celeritas::tcp_session_run<SocketType>::run()
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
celeritas::session_run::void_awaitable_type celeritas::tcp_session_run<SocketType>::handle_one_message()
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
    const auto callback = session_callback_.get_network_message_callback().lock();
    if (callback != nullptr)
    {
        callback->call_back(header, std::move(buffer_guard));
    }
}

template <typename SocketType>
typename celeritas::tcp_session_run<SocketType>::read_awaitable_type celeritas::tcp_session_run<SocketType>::read_data_with_timeout(boost::asio::mutable_buffer buffer)
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

    auto [read_error_code, bytes_read] = co_await boost::asio::async_read(socket_, buffer, await_token);

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