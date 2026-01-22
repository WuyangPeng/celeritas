#include "listener.h"
#include "common/common_constant.h"
#include "common/core/noexcept_safe_call_and_log.h"

celeritas::listener::listener(const any_io_executor& any_io_executor,
                              network_message_callback_weak_ptr callback,
                              std::string game_server_id,
                              const server_network_type server_network_type)
    : any_io_executor_{ any_io_executor },
      network_message_callback_{ std::move(callback) },
      game_server_id_{ std::move(game_server_id) },
      server_network_type_{ server_network_type }
{
}

void celeritas::listener::start()
{
    boost::asio::co_spawn(any_io_executor_,
                          noexcept_safe_call_and_log_awaitable([self = shared_from_this()] {
                                                                   return self->accept_connections();
                                                               },
                                                               network_channel,
                                                               "listener start error: "),
                          boost::asio::detached);
}

celeritas::server_network_type celeritas::listener::get_server_network_type() const noexcept
{
    return server_network_type_;
}

std::string celeritas::listener::get_game_server_id() const
{
    return game_server_id_;
}

celeritas::listener::network_message_callback_weak_ptr celeritas::listener::get_network_message_callback()
{
    return network_message_callback_;
}

celeritas::listener::any_io_executor celeritas::listener::get_any_io_executor()
{
    return any_io_executor_;
}

