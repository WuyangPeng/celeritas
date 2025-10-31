#pragma once

#include "network_internal_fwd.h"
#include "tcp_session_run.h"
#include "common/buffer_guard.h"
#include "common/buffer_pool.h"
#include "common/common_fwd.h"
#include "common/logger.h"
#include "network/message_header.h"
#include "network/network_message_callback.h"

template <typename SocketType>
celeritas::tcp_session_run<SocketType>::tcp_session_run(socket_type& socket, const int64_t session_id, session_callback session_callback)
    : socket_{ socket }, session_id_{ session_id }, session_callback_{ std::move(session_callback) }
{
}

template <typename SocketType>
void celeritas::tcp_session_run<SocketType>::do_start()
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
            break;
        }
        catch (const std::exception& error)
        {
            LOG_CHANNEL(network_channel, error) << "An unexpected error occurred: " << error.what();
            break;
        }
        catch (...)
        {
            LOG_CHANNEL(network_channel, fatal) << "Listener unknown error.";
            break;
        }
    }

    session_callback_.remove_session(session_id_);
}

template <typename SocketType>
auto celeritas::tcp_session_run<SocketType>::setup_timeout_cancellation_slot(steady_timer_type& steady_timer, cancellation_signal_type& cancel_signal)
{
    co_spawn(socket_.get_executor(), [&]() -> boost::asio::awaitable<void> {
                 auto await_token = boost::asio::as_tuple(boost::asio::use_awaitable);
                 if (auto [error_code] = co_await steady_timer.async_wait(await_token);
                     error_code != boost::asio::error::operation_aborted)
                 {
                     cancel_signal.emit(boost::asio::cancellation_type::all);
                 }
                 co_return;
             },
             boost::asio::detached);

    return boost::asio::as_tuple(boost::asio::bind_cancellation_slot(cancel_signal.slot(), boost::asio::use_awaitable));
}

template <typename SocketType>
celeritas::session_run::void_awaitable_type celeritas::tcp_session_run<SocketType>::handle_one_message()
{
    // 读取消息头
    message_header header{};
    co_await read_data_with_timeout(boost::asio::buffer(&header, sizeof(header)));

    if (auto buffer_guard_optional = get_buffer_guard(header))
    {
        auto buffer_guard = std::move(*buffer_guard_optional);
        co_await read_data_with_timeout(boost::asio::buffer(buffer_guard.get(), buffer_guard.get_effective_size()));

        LOG_CHANNEL(network_channel, debug) << "Received message of type: "
                                            << header.get_header_type()
                                            << ",header size:"
                                            << header.get_header_size()
                                            << ",body size:"
                                            << header.get_body_size();

        call_back(header, std::move(buffer_guard));
    }
}

template <typename SocketType>
celeritas::tcp_session_run<SocketType>::read_awaitable_type celeritas::tcp_session_run<SocketType>::read_data_with_timeout(mutable_buffer_type buffer)
{
    steady_timer_type timer{ socket_.get_executor(), std::chrono::steady_clock::now() + timeout_seconds };
    boost::asio::cancellation_signal cancel_signal{};

    auto await_token = setup_timeout_cancellation_slot(timer, cancel_signal);

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

template <typename SocketType>
void celeritas::tcp_session_run<SocketType>::call_back(const message_header& message_header, buffer_guard buffer_guard)
{
    const auto session = get_session();

    if (const auto callback = session_callback_.get_network_message_callback_shared_ptr();
        callback != nullptr && session != nullptr)
    {
        callback->call_back(message_header, std::move(buffer_guard), session);
    }
}

template <typename SocketType>
celeritas::tcp_session_run<SocketType>::buffer_guard_optional_type celeritas::tcp_session_run<SocketType>::get_buffer_guard(message_header& message_header)
{
    // 转换字节序
    message_header.network_to_host();
    const auto total_size = message_header.get_total_size();
    if (total_size == 0)
    {
        return std::nullopt;
    }

    if (!message_header.is_effective())
    {
        LOG_CHANNEL(network_channel, error) << "oversized msg, drop connection";
        throw boost::system::system_error(boost::asio::error::message_size);
    }

    return buffer_guard{ buffer_pool::acquire(total_size), total_size };
}