#include "websocket_listener.h"
#include "common/common_fwd.h"
#include "common/logger.h"

celeritas::websocket_listener::websocket_listener(io_context_type& io_context,
                                                  network_message_callback_weak_ptr callback,
                                                  std::string game_server_id,
                                                  const int port)
    : base_type{ io_context, std::move(callback), std::move(game_server_id) },
      acceptor_{ io_context, boost::asio::ip::tcp::endpoint{ boost::asio::ip::tcp::v4(), boost::numeric_cast<uint_least16_t>(port) } }
{
    set_option(port);
}

void celeritas::websocket_listener::set_option(int port)
{
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true));

    LOG_CHANNEL(network_channel, info) << "web socket listening on port " << port << "...";
}

void celeritas::websocket_listener::stop()
{
    set_stop();

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
    while (is_running())
    {
        try
        {
            co_await handle_connection();
        }
        catch (const boost::system::system_error& error)
        {
            if (error.code() != boost::asio::error::operation_aborted)
            {
                LOG_CHANNEL(network_channel, warning) << "web socket listener error: " << error.what();
            }
        }
        catch (...)
        {
            LOG_CHANNEL(network_channel, error) << "web socket listener unknown error.";
        }
    }

    LOG_CHANNEL(network_channel, info) << "web socket listener stopped.";
}

celeritas::websocket_listener::void_awaitable_type celeritas::websocket_listener::handle_connection()
{
    // 等待新连接
    auto result = co_await acceptor_.async_accept(boost::asio::as_tuple(boost::asio::use_awaitable));

    if (const auto error = std::get<0>(result))
    {
        if (error != boost::asio::error::operation_aborted)
        {
            LOG_CHANNEL(network_channel, warning) << "websocket listener accept error: " << error.message();
        }
    }
    else
    {
        start_new_session(std::move(std::get<1>(result)));
    }
}

void celeritas::websocket_listener::start_new_session(socket_type socket)
{
    const auto current_session_id = get_next_session_id();

    LOG_CHANNEL(network_channel, info) << "Accepted new web socket connection [" << current_session_id << "] from: " << socket.remote_endpoint();

    const auto session = std::make_shared<websocket_session>(std::move(socket), current_session_id, get_game_server_id(), get_session_callback());
    add_session(session);

    session->start();
}