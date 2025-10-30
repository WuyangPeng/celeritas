#include "websocket_listener_accept.h"
#include "common/logger.h"

celeritas::websocket_listener_accept::websocket_listener_accept(acceptor_type& acceptor, std::string game_server_id, network_message_callback_weak_ptr callback)
    : base_type{}, acceptor_{ acceptor }, game_server_id_{ std::move(game_server_id) }, callback_{ std::move(callback) }
{
}

void celeritas::websocket_listener_accept::stop()
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

celeritas::listener_sessions::void_awaitable_type celeritas::websocket_listener_accept::accept_connections()
{
    while (is_running())
    {
        co_await do_accept_connections();
    }

    LOG_CHANNEL(network_channel, info) << "web socket listener stopped.";
}

celeritas::listener_sessions::void_awaitable_type celeritas::websocket_listener_accept::do_accept_connections()
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

celeritas::listener_sessions::void_awaitable_type celeritas::websocket_listener_accept::handle_connection()
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

void celeritas::websocket_listener_accept::start_new_session(socket_type socket)
{
    const auto current_session_id = get_next_session_id();

    LOG_CHANNEL(network_channel, info) << "Accepted new web socket connection [" << current_session_id << "] from: " << socket.remote_endpoint();

    const auto session = std::make_shared<websocket_session>(std::move(socket), current_session_id, game_server_id_, session_callback{ shared_from_this(), callback_ });
    add_session(session);

    session->start();
}