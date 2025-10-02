#include "session_base.tpp"
#include "tcp_listener.h"
#include "common/logger.h"

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>

celeritas::tcp_listener::tcp_listener(boost::asio::io_context& io_context, const int port, const network_message_callback_shared_ptr& callback)
    : io_context_{ io_context },
      acceptor_{ io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port) },
      network_message_callback_{ callback },
      is_running_{ true },
      sessions_{},
      session_id_{ 0 }
{
    LOG_CHANNEL(network_channel, info) << "Listening on port " << port << "...";
}

void celeritas::tcp_listener::start()
{
    co_spawn(io_context_, [this] {
                 return accept_connections();
             },
             boost::asio::detached);
}

void celeritas::tcp_listener::stop()
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

void celeritas::tcp_listener::remove_session(long session_id)
{
    sessions_.erase(session_id);
}

// 协程：接受连接
celeritas::tcp_listener::void_awaitable_type celeritas::tcp_listener::accept_connections()
{
    while (is_running_)
    {
        try
        {
            co_await handle_connection();
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

celeritas::tcp_listener::void_awaitable_type celeritas::tcp_listener::handle_connection()
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
        auto session = std::make_shared<session_type>(std::move(socket), ++session_id_, network_message_callback_, shared_from_this());
        session->start();

        sessions_.insert({ session->get_session_id(), std::move(session) });
    }
}
