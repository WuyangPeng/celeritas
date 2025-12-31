#include "websocket_session_handle_one_message.h"
#include "websocket_session_handle_session.h"
#include "common/logging/logger.h"

celeritas::websocket_session_handle_session::websocket_session_handle_session(web_socket_stream_type& web_socket,
                                                                              const int64_t session_id,
                                                                              network_message_callback_weak_ptr callback,
                                                                              session_weak_ptr session)
    : web_socket_{ web_socket }, session_id_{ session_id }, callback_{ std::move(callback) }, session_{ std::move(session) }
{
}

celeritas::websocket_session_handle_session::void_awaitable_type celeritas::websocket_session_handle_session::run()
{
    while (web_socket_.is_open())
    {
        try
        {
            co_await handle_one_message();
        }
        catch (const boost::system::system_error& error)
        {
            if (error.code() == boost::beast::websocket::error::closed ||
                error.code() == boost::asio::error::operation_aborted)
            {
                LOG_CHANNEL(network_channel, info) << "WS Session [" << session_id_ << "] closed gracefully.";
            }
            else
            {
                LOG_CHANNEL(network_channel, warning) << "WS Session [" << session_id_ << "] run error: " << error.what();
            }
            break;
        }
        catch (const std::exception& error)
        {
            LOG_CHANNEL(network_channel, error) << "WS Session [" << session_id_ << "] error: " << error.what();
        }
        catch (...)
        {
            LOG_CHANNEL(network_channel, fatal) << "WS Session [" << session_id_ << "] unknown error.";
            break;
        }
    }
}

celeritas::websocket_session_handle_session::void_awaitable_type celeritas::websocket_session_handle_session::handle_one_message()
{
    co_await web_socket_.async_accept(boost::asio::use_awaitable);

    LOG_CHANNEL(network_channel, info) << "socket session [" << session_id_ << "] upgraded to websocket.";

    const websocket_session_handle_one_message handler{ web_socket_, session_id_, callback_, session_ };
    co_await handler.run();
}


