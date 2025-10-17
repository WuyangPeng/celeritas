#pragma once

#include "session_base.h"
#include "detail/tcp_session_run.tpp"
#include "detail/tcp_session_write.tpp"

template <typename SocketType>
celeritas::session_base<SocketType>::session_base(socket_type socket,
                                                  const long session_id,
                                                  std::string game_server_id,
                                                  session_callback session_callback)
    : base_type{ session_id, std::move(session_callback) },
      socket_{ std::move(socket) },
      session_write_{ std::make_shared<tcp_session_write<socket_type> >(socket_) },
      session_run_{ std::make_shared<tcp_session_run<socket_type> >(socket_, get_session_callback()) },
      game_server_id_{ std::move(game_server_id) }
{
}

template <typename SocketType>
void celeritas::session_base<SocketType>::start()
{
    session_run_->start(shared_from_this());
}

template <typename SocketType>
bool celeritas::session_base<SocketType>::is_open() const
{
    return socket_.is_open();
}

template <typename SocketType>
void celeritas::session_base<SocketType>::write(buffer_guard data)
{
    session_write_->write(std::move(data));
}
