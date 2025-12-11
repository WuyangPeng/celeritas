#include "websocket_listener.h"
#include "common/logger.h"
#include "detail/websocket_listener_accept.h"

celeritas::websocket_listener::websocket_listener(io_context_type& io_context,
                                                  network_message_callback_weak_ptr callback,
                                                  std::string game_server_id,
                                                  const int port,
                                                  const server_network_type server_network_type)
    : base_type{ io_context, std::move(callback), std::move(game_server_id), server_network_type },
      acceptor_{ io_context, boost::asio::ip::tcp::endpoint{ boost::asio::ip::tcp::v4(), boost::numeric_cast<uint_least16_t>(port) } },
      listener_accept_{ std::make_shared<websocket_listener_accept>(acceptor_, server_network_type, get_game_server_id(), get_network_message_callback()) }
{
    set_option(port);
}

void celeritas::websocket_listener::stop()
{
    listener_accept_->stop();
}

celeritas::websocket_listener::void_awaitable_type celeritas::websocket_listener::accept_connections()
{
    co_return co_await listener_accept_->accept_connections();
}

celeritas::listener::session_shared_ptr celeritas::websocket_listener::get_session(int64_t id)
{
    return listener_accept_->get_session(id);
}

void celeritas::websocket_listener::set_option(const int port)
{
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true));

    LOG_CHANNEL(network_channel, info) << "web socket listening on port " << port << "...";
}
