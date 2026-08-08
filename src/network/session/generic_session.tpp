#pragma once

#include "generic_session.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "network/session_helper/detail/tcp_session_run.tpp"
#include "network/session_helper/detail/tcp_session_write.tpp"

template <typename SocketType>
celeritas::generic_session<SocketType>::generic_session(socket_type socket,
                                                        const server_network_type server_network_type,
                                                        const long session_id,
                                                        std::string game_server_id,
                                                        session_callback session_callback)
    : base_type{ server_network_type, session_id, std::move(session_callback) },
      socket_{ std::move(socket) },
      session_write_{ std::make_shared<tcp_session_write<socket_type> >(socket_) },
      session_run_{ std::make_shared<tcp_session_run<socket_type> >(socket_, session_id, get_session_callback()) },
      game_server_id_{ std::move(game_server_id) }
{
}

template <typename SocketType>
celeritas::generic_session<SocketType>::~generic_session() noexcept
{
    noexcept_safe_call_and_log([this] {
                                   session_run_->send_offline_message();
                               },
                               network_channel,
                               "send offline message error: ");

    noexcept_safe_call_and_log([this] {
                                   this->stop();
                               },
                               network_channel,
                               "closed tcp session error: ");
}

template <typename SocketType>
void celeritas::generic_session<SocketType>::start()
{
    session_run_->start(shared_from_this());
}

template <typename SocketType>
celeritas::session_base::void_awaitable_type celeritas::generic_session<SocketType>::start_awaitable()
{
    co_await session_run_->start_awaitable(shared_from_this());
}

template <typename SocketType>
void celeritas::generic_session<SocketType>::stop()
{
    if (is_open())
    {
        socket_.close();
    }
}

template <typename SocketType>
bool celeritas::generic_session<SocketType>::is_open() const
{
    return socket_.is_open();
}

template <typename SocketType>
bool celeritas::generic_session<SocketType>::is_full() const
{
    return session_write_->is_full();
}

template <typename SocketType>
celeritas::session::any_io_executor celeritas::generic_session<SocketType>::get_any_io_executor()
{
    return socket_.get_executor();
}

template <typename SocketType>
void celeritas::generic_session<SocketType>::do_write(buffer_guard data)
{
    session_write_->write(std::move(data));
}

template <typename SocketType>
celeritas::session_base::void_awaitable_type celeritas::generic_session<SocketType>::do_write_immediately(buffer_guard data)
{
    co_await session_write_->write_immediately(std::move(data), shared_from_this());
}

template <typename SocketType>
std::string celeritas::generic_session<SocketType>::get_remote_ip_address() const
{
    try
    {
        return socket_.remote_endpoint().address().to_string();
    }
    catch (const std::exception& e)
    {
        LOG_CHANNEL(network_channel, error) << "get remote ip address error: " << e.what();
        return "";
    }
    catch (...)
    {
        LOG_CHANNEL(network_channel, error) << "get remote ip address error: unknown exception";
        return "";
    }
}
