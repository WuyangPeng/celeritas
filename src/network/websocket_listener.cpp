#include "websocket_listener.h"
#include "common/logger.h"
#include "common/common_fwd.h"

celeritas::websocket_listener::websocket_listener(boost::asio::io_context& io_context, int port, const network_message_callback_shared_ptr& callback)
    : io_context_{ io_context },
      acceptor_{ io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port) },
      network_message_callback_{ callback },
      is_running_{ true },
      sessions_{},
      session_id_{ 0 }
{
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true));
    LOG_CHANNEL(network_channel, info) << "WebSocket Listening on port " << port << "...";
}

void celeritas::websocket_listener::start()
{
    boost::asio::co_spawn(io_context_, [this] {
        return accept_connections();
    }, boost::asio::detached);
}

void celeritas::websocket_listener::stop()
{
    is_running_ = false;

    // 取消 acceptor
    boost::system::error_code error_code{};
    acceptor_.cancel(error_code);
    if (error_code)
    {
        LOG_CHANNEL(network_channel, warning) << "Failed to cancel acceptor: " << error_code.message();
    }
}

celeritas::websocket_listener::void_awaitable_type celeritas::websocket_listener::accept_connections()
{
    while (is_running_)
    {
        try
        {
            co_await handle_connection();
        }
        catch (const boost::system::system_error& error)
        {
            if (error.code() != boost::asio::error::operation_aborted)
            {
                LOG_CHANNEL(network_channel, warning) << "WS Listener error: " << error.what();
            }
        }
        catch (...)
        {
            LOG_CHANNEL(network_channel, error) << "WS Listener unknown error.";
        }
    }
    LOG_CHANNEL(network_channel, info) << "WS Listener stopped.";
}

celeritas::websocket_listener::void_awaitable_type celeritas::websocket_listener::handle_connection()
{
    // 等待新连接
    auto result = co_await acceptor_.async_accept(boost::asio::as_tuple(boost::asio::use_awaitable));

    if (auto error = std::get<0>(result))
    {
        if (error != boost::asio::error::operation_aborted)
        {
            LOG_CHANNEL(network_channel, warning) << "WS Listener accept error: " << error.message();
        }
    }
    else
    {
        // 成功接受连接
        auto socket = std::move(std::get<1>(result));
        long current_session_id = ++session_id_;

        LOG_CHANNEL(network_channel, info) << "Accepted new WS connection [" << current_session_id
                                              << "] from: " << socket.remote_endpoint();

        // 创建新的 websocket_session
        auto new_session = std::make_shared<session_type>(
            std::move(socket), current_session_id, network_message_callback_);

        // 将 session 存储起来
        sessions_[current_session_id] = new_session;

        // 启动 websocket_session 协程
        new_session->start();
    }
}