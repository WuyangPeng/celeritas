#include "http_listener.h"
#include "common/logger.h"
#include "detail/http_listener_accept.h"

celeritas::http_listener::http_listener(io_context_type& io_context,
                                        network_message_callback_weak_ptr callback,
                                        std::string game_server_id,
                                        const int port)
    : base_type{ io_context, std::move(callback), std::move(game_server_id) },
      acceptor_{ io_context, boost::asio::ip::tcp::endpoint{ boost::asio::ip::tcp::v4(), boost::numeric_cast<uint_least16_t>(port) } },
      listener_accept_{ std::make_shared<http_listener_accept>(acceptor_, get_game_server_id(), get_network_message_callback()) }
{
    set_option(port);
}

void celeritas::http_listener::stop()
{
    listener_accept_->stop();
}

celeritas::http_listener::void_awaitable_type celeritas::http_listener::accept_connections()
{
    co_return co_await listener_accept_->accept_connections();
}

void celeritas::http_listener::set_option(const int port)
{
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true));

    LOG_CHANNEL(network_channel, info) << "http listening on port " << port << "...";
}
