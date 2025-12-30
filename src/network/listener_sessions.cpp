#include "listener_sessions.h"
#include "session_base.h"
#include "common/core_utilities/celeritas_error.h"
#include "common/logger.h"

celeritas::listener_sessions::listener_sessions(const server_network_type server_network_type) noexcept
    : server_network_type_{ server_network_type }
{
}

void celeritas::listener_sessions::remove_session(const int64_t session_id)
{
    if (const auto iter = sessions_.find(session_id);
        iter != sessions_.cend())
    {
        LOG_CHANNEL(network_channel, debug) << "Listener sessions remove: session id =" << session_id;

        iter->second->stop();
        sessions_.erase(iter);
    }
}

void celeritas::listener_sessions::set_stop()
{
    is_running_ = false;
}

bool celeritas::listener_sessions::is_running() const noexcept
{
    return is_running_;
}

int64_t celeritas::listener_sessions::get_next_session_id() noexcept
{
    return ++session_id_;
}

void celeritas::listener_sessions::add_session(const session_shared_ptr& session)
{
    sessions_[session->get_session_id()] = session;
}

celeritas::listener_sessions::session_shared_ptr celeritas::listener_sessions::get_session(const int64_t id)
{
    const auto iter = sessions_.find(id);
    if (iter == sessions_.cend())
    {
        throw celeritas_error{ "no session found for id {}", id };
    }

    return iter->second;
}

celeritas::server_network_type celeritas::listener_sessions::get_server_network_type() const noexcept
{
    return server_network_type_;
}

bool celeritas::listener_sessions::write(const std::string& server_type, const std::string& instance_id, const header& header, const protobuf_message& request)
{
    auto to_write = false;
    for (const auto& element : sessions_)
    {
        if (element.second->get_instance_id() == instance_id)
        {
            element.second->write(header, request);
            to_write = true;
        }
    }

    return to_write;
}