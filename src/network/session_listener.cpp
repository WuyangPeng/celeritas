#include "session_listener.h"

celeritas::session_listener::session_listener(io_context_type& io_context, network_message_callback_weak_ptr callback, std::string game_server_id)
    : base_type{ io_context, std::move(callback), std::move(game_server_id) },
      is_running_{ true },
      sessions_{},
      session_id_{ 0 }
{
}

void celeritas::session_listener::remove_session(int64_t session_id)
{
    sessions_.erase(session_id);
}

void celeritas::session_listener::set_stop()
{
    is_running_ = false;
}

bool celeritas::session_listener::is_running() const noexcept
{
    return is_running_;
}

int64_t celeritas::session_listener::get_next_session_id() noexcept
{
    return ++session_id_;
}

void celeritas::session_listener::add_session(const session_shared_ptr& session)
{
    sessions_[session->get_session_id()] = session;
}