#pragma once

#include "common/buffer/buffer_guard.h"
#include "network/core/network_message_callback.h"

namespace celeritas
{
    class mock_network_message_callback : public network_message_callback
    {
    public:
        using class_type = mock_network_message_callback;
        using base_type = network_message_callback;

        void call_back(const message_header& message_header, buffer_guard buffer_guard, const session_shared_ptr& session) override;

        void call_back(const std::string& path, const urls_params_view_type& params, const session_shared_ptr& session) override;

        void call_back(const std::string& path, const std::string& params, const session_shared_ptr& session) override;

        void send_offline_message(int64_t session_id) override;
    };
}