#include "listener.h"

celeritas::listener::listener(io_context_type& io_context, network_message_callback_weak_ptr callback, std::string game_server_id)
    : io_context_{ io_context }, network_message_callback_{ std::move(callback) }, game_server_id_{ std::move(game_server_id) }
{
}

void celeritas::listener::start()
{
    boost::asio::co_spawn(io_context_,
                          [this] {
                              return this->accept_connections();
                          }, boost::asio::detached);
}

std::string celeritas::listener::get_game_server_id() const
{
    return game_server_id_;
}

celeritas::listener::network_message_callback_weak_ptr celeritas::listener::get_network_message_callback()
{
    return network_message_callback_;
}

celeritas::listener::io_context_type& celeritas::listener::get_io_context()
{
    return io_context_;
}