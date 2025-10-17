#pragma once

#include "network/network_fwd.h"
#include "common/buffer_guard.h"

namespace celeritas
{
    class network_message_callback : public std::enable_shared_from_this<network_message_callback>
    {
    public:
        using class_type = network_message_callback;
        using session_shared_ptr = std::shared_ptr<session>;

        network_message_callback() noexcept = default;

        virtual ~network_message_callback() noexcept = default;

        network_message_callback(const network_message_callback& rhs) = default;

        network_message_callback& operator=(const network_message_callback& rhs) = default;

        network_message_callback(network_message_callback&& rhs) = default;

        network_message_callback& operator=(network_message_callback&& rhs) = default;

        virtual void call_back(const message_header& message_header, buffer_guard buffer_guard, const session_shared_ptr& session) = 0;
    };
}