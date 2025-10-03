#pragma once

#include "network_fwd.h"

#include <memory>

namespace celeritas
{
    class session_callback
    {
    public:
        using class_type = session_callback;
        using listener_weak_ptr = std::weak_ptr<listener>;
        using network_message_callback_weak_ptr = std::weak_ptr<network_message_callback>;

        session_callback(listener_weak_ptr listener, network_message_callback_weak_ptr network_message_callback);

        void remove_session(int64_t session_id);

        [[nodiscard]] network_message_callback_weak_ptr get_network_message_callback();

    private:
        listener_weak_ptr listener_;
        network_message_callback_weak_ptr network_message_callback_;
    };
}