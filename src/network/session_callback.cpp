#include "listener.h"
#include "session_callback.h"

celeritas::session_callback::session_callback(listener_weak_ptr listener, network_message_callback_weak_ptr network_message_callback)
    : listener_{ std::move(listener) }, network_message_callback_{ std::move(network_message_callback) }
{
}

void celeritas::session_callback::remove_session(int64_t session_id)
{
    if (const auto listener = listener_.lock();
        listener != nullptr)
    {
        listener->remove_session(session_id);
    }
}