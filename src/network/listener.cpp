#include "listener.h"
#include "session.h"
#include "common/logger.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>

celeritas::listener::listener(boost::asio::io_context& io_context, unsigned short port)
    : io_context_{ io_context }, acceptor_{ io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port) }
{
    LOG(info) << "Listening on port " << port << "...";
}

// 启动协程来接受连接
void celeritas::listener::start()
{
    co_spawn(io_context_,
             [this] {
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
            auto [error, socket] = co_await acceptor_.async_accept(boost::asio::use_awaitable);
            if (error)
            {
                LOG(warning) << "Listener error: " << error.what();
            }
            else
            {
                LOG(info) << "Accepted new connection from: " << socket.remote_endpoint();

                // 为新连接创建一个会话，并启动
                std::make_shared<session>(std::move(socket))->start();
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
