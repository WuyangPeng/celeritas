#pragma once

#include "listener_sessions.h"
#include "network_fwd.h"

#include <memory>

namespace celeritas
{
    class session_callback
    {
    public:
        using class_type = session_callback;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;
        using listener_accept_weak_ptr = std::weak_ptr<listener_sessions>;
        using network_message_callback_shared_ptr = std::shared_ptr<network_message_callback>;

        session_callback(listener_accept_weak_ptr listener_accept, network_message_callback_weak_ptr network_message_callback);

        void remove_session(int64_t session_id);

        [[nodiscard]] network_message_callback_weak_ptr get_network_message_callback();

        [[nodiscard]] network_message_callback_shared_ptr get_network_message_callback_shared_ptr();

    private:
        listener_accept_weak_ptr listener_accept_;
        network_message_callback_weak_ptr network_message_callback_;
    };
}