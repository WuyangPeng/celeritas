#include "tcp_listener_accept.h"
#include "common/logger.h"
#include "common/common_fwd.h"
#include "network/session_base.tpp"
#include "network/session_callback.h"

#include <boost/asio/as_tuple.hpp>
#include <boost/asio/use_awaitable.hpp>

celeritas::tcp_listener_accept::tcp_listener_accept(acceptor_type& acceptor, std::string game_server_id, network_message_callback_weak_ptr callback)
    : base_type{}, acceptor_{ acceptor }, game_server_id_{ std::move(game_server_id) }, callback_{ std::move(callback) }
{
}

void celeritas::tcp_listener_accept::stop()
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

celeritas::listener_sessions::void_awaitable_type celeritas::tcp_listener_accept::accept_connections()
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

celeritas::tcp_listener_accept::void_awaitable_type celeritas::tcp_listener_accept::handle_connection()
{
    // 等待新连接
    auto result = co_await acceptor_.async_accept(boost::asio::as_tuple(boost::asio::use_awaitable));
    if (const auto& error = std::get<0>(result))
    {
        LOG_CHANNEL(network_channel, warning) << "Listener error: " << error.message();
    }
    else
    {
        start_new_session(std::move(std::get<1>(result)));
    }
}

void celeritas::tcp_listener_accept::start_new_session(socket_type socket)
{
    const auto current_session_id = get_next_session_id();

    LOG_CHANNEL(network_channel, info) << "Accepted new connection from: " << socket.remote_endpoint();

    // 为新连接创建一个会话，并启动
    const auto session = std::make_shared<session_type>(std::move(socket), current_session_id, game_server_id_, session_callback{ shared_from_this(), callback_ });

    add_session(session);

    session->start();
}