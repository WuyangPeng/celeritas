#include "common/logger.h"
#include "listener.h"
#include "session.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>

celeritas::listener::listener(boost::asio::io_context& io_context, unsigned short port, message_handler_type handler)
    : io_context_{ io_context },
      acceptor_{ io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port) },
      message_handler_{ handler }
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

// 协程：接受连接
boost::asio::awaitable<void> celeritas::listener::accept_connections()
{
    for (;;)
    {
        try
        {
            // 等待新连接
            auto result = co_await acceptor_.async_accept(boost::asio::as_tuple(boost::asio::use_awaitable));
            if (auto error = std::get<0>(result))
            {
                LOG(warning) << "Listener error: " << error.message();
            }
            else
            {
                auto socket = std::move(std::get<1>(result));
                LOG(info) << "Accepted new connection from: " << socket.remote_endpoint();

                // 为新连接创建一个会话，并启动
                std::make_shared<session>(std::move(socket), message_handler_)->start();
            }
        }
        catch (const boost::system::system_error& error)
        {
            LOG(warning) << "Listener error: " << error.what();
        }
        catch (const std::exception& error)
        {
            LOG(warning) << "Listener error: " << error.what();
        }
        catch (...)
        {
            LOG(error) << "Listener unknown error.";
        }
    }
}
