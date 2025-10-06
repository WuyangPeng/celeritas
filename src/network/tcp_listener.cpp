#include "session_base.tpp"
#include "tcp_listener.h"
#include "common/logger.h"

#include <boost/asio/use_awaitable.hpp>

celeritas::tcp_listener::tcp_listener(io_context_type& io_context,
                                      network_message_callback_weak_ptr callback,
                                      std::string game_server_id,
                                      const int port)
    : base_type{ io_context, std::move(callback), std::move(game_server_id) },
      acceptor_{ io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port) }
{
    LOG_CHANNEL(network_channel, info) << "Listening on port " << port << "...";
}

void celeritas::tcp_listener::stop()
{
    set_stop();

    // 取消 acceptor，这会立即中断 async_accept 调用
    boost::system::error_code error_code{};
    acceptor_.cancel(error_code);
    if (error_code)
    {
        LOG_CHANNEL(network_channel, warning) << "Failed to cancel acceptor: " << error_code.message();
    }
}

// 协程：接受连接
celeritas::tcp_listener::void_awaitable_type celeritas::tcp_listener::accept_connections()
{
    while (is_running())
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
        start_new_session(std::move(std::get<1>(result)));
    }
}

void celeritas::tcp_listener::start_new_session(socket_type socket)
{
    const auto current_session_id = get_next_session_id();

    LOG_CHANNEL(network_channel, info) << "Accepted new connection from: " << socket.remote_endpoint();

    // 为新连接创建一个会话，并启动
    auto session = std::make_shared<session_type>(std::move(socket), current_session_id, get_game_server_id(), get_session_callback());
    session->start();

    add_session(session);
}
