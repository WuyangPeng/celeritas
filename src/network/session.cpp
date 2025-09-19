#include "common/buffer_guard.h"
#include "common/buffer_pool.h"
#include "common/logger.h"
#include "message_header.h"
#include "session.h"
#include "detail/network_internal_fwd.h"

celeritas::session::session(socket_type socket, message_handler_type handler)
    : socket_{ std::move(socket) }, message_handler_{ std::move(handler) }
{
}

void celeritas::session::start()
{
    co_spawn(socket_.get_executor(), [self = shared_from_this()] {
        return self->handle_session();
    },
             boost::asio::detached);
}

celeritas::session::awaitable_type celeritas::session::handle_session()
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

celeritas::session::read_awaitable_type celeritas::session::read_data_with_timeout(boost::asio::mutable_buffer buffer)
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

celeritas::session::awaitable_type celeritas::session::handle_one_message()
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
