#include "session_base.tpp"
#include "tcp_listener.h"
#include "common/logger.h"
#include "detail/tcp_listener_accept.h"

celeritas::tcp_listener::tcp_listener(io_context_type& io_context,
                                      network_message_callback_weak_ptr callback,
                                      std::string game_server_id,
                                      const int port)
    : base_type{ io_context, std::move(callback), std::move(game_server_id) },
      acceptor_{ io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port) },
      listener_accept_{ std::make_shared<tcp_listener_accept>(acceptor_, get_game_server_id(), get_network_message_callback()) }
{
    LOG_CHANNEL(network_channel, info) << "Listening on port " << port << "...";
}

void celeritas::tcp_listener::stop()
{
    listener_accept_->stop();
}

// 协程：接受连接
celeritas::tcp_listener::void_awaitable_type celeritas::tcp_listener::accept_connections()
{
    co_return co_await listener_accept_->accept_connections();
}
