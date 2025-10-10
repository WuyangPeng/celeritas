#include "listener.h"
#include "session_callback.h"

#include <utility>

celeritas::session_callback::session_callback(listener_accept_weak_ptr listener_accept, network_message_callback_weak_ptr network_message_callback)
    : listener_accept_{ std::move(listener_accept) }, network_message_callback_{ std::move(network_message_callback) }
{
}

void celeritas::session_callback::remove_session(int64_t session_id)
{
    if (const auto listener_accept = listener_accept_.lock();
        listener_accept != nullptr)
    {
        listener_accept->remove_session(session_id);
    }
}

celeritas::session_callback::network_message_callback_weak_ptr celeritas::session_callback::get_network_message_callback()
{
    return network_message_callback_;
}

