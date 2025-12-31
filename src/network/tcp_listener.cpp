#include "tcp_listener.h"
#include "common/logging/logger.h"
#include "detail/tcp_listener_accept.h"

celeritas::tcp_listener::tcp_listener(io_context_type& io_context,
                                      network_message_callback_weak_ptr callback,
                                      std::string game_server_id,
                                      const int port,
                                      const server_network_type server_network_type)
    : base_type{ io_context, std::move(callback), std::move(game_server_id), server_network_type },
      acceptor_{ io_context, boost::asio::ip::tcp::endpoint{ boost::asio::ip::tcp::v4(), boost::numeric_cast<uint_least16_t>(port) } },
      listener_accept_{ std::make_shared<tcp_listener_accept>(acceptor_, server_network_type, get_game_server_id(), get_network_message_callback()) }
{
    LOG_CHANNEL(network_channel, info) << "listening on port " << port << "...";
}

void celeritas::tcp_listener::stop()
{
    listener_accept_->stop();
}

celeritas::tcp_listener::void_awaitable_type celeritas::tcp_listener::accept_connections()
{
    co_return co_await listener_accept_->accept_connections();
}

celeritas::listener::session_shared_ptr celeritas::tcp_listener::get_session(int64_t id)
{
    return listener_accept_->get_session(id);
}

bool celeritas::tcp_listener::write(const std::string& server_type, const std::string& instance_id, const header& header, const protobuf_message& request)
{
    return listener_accept_->write(server_type, instance_id, header, request);
}
