#include "common/logger.h"
#include "listener.h"
#include "session.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>

celeritas::listener::listener(boost::asio::io_context& io_context, uint16_t port, message_handler_type handler)
    : io_context_{ io_context },
      acceptor_{ io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port) },
      message_handler_{ std::move(handler) },
      is_running_{ true }
{
    LOG(info) << "Listening on port " << port << "...";
}

// 启动协程来接受连接
void celeritas::listener::start()
{
    co_spawn(io_context_, [this] {
        return accept_connections();
    },
             boost::asio::detached);
}

void celeritas::listener::stop()
{
    // 设置标志，通知协程退出
    is_running_ = false;

    // 取消 acceptor，这会立即中断 async_accept 调用
    boost::system::error_code error_code{};
    acceptor_.cancel(error_code);
    if (error_code)
    {
        LOG_CHANNEL(network_channel, warning) << "Failed to cancel acceptor: " << error_code.message();
    }
}

// 协程：接受连接
boost::asio::awaitable<void> celeritas::listener::accept_connections()
{
    while (is_running_)
    {
        try
        {
            // 等待新连接
            auto result = co_await acceptor_.async_accept(boost::asio::as_tuple(boost::asio::use_awaitable));
            if (auto error = std::get<0>(result))
            {
                LOG_CHANNEL(network_channel, warning) << "Listener error: " << error.message();
            }
            else
            {
                auto socket = std::move(std::get<1>(result));
                LOG_CHANNEL(network_channel, info) << "Accepted new connection from: " << socket.remote_endpoint();

                // 为新连接创建一个会话，并启动
                std::make_shared<session>(std::move(socket), message_handler_)->start();
            }
        }
        catch (const boost::system::system_error& error)
        {
            LOG_CHANNEL(network_channel, warning) << "Listener error: " << error.what();
        }
        catch (const std::exception& error)
        {
            LOG_CHANNEL(network_channel, warning) << "Listener error: " << error.what();
        }
        catch (...)
        {
            LOG_CHANNEL(network_channel, error) << "Listener unknown error.";
        }
    }
}
