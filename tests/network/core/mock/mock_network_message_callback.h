#pragma once

#include "common/buffer/buffer_guard.h"
#include "network/core/message_header.h"
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

        [[nodiscard]] bool was_message_header_called() const;

        [[nodiscard]] bool was_url_params_called() const;

        [[nodiscard]] bool was_string_params_called() const;

        [[nodiscard]] bool was_offline_called() const;

        [[nodiscard]] const message_header& get_last_message_header() const;

        [[nodiscard]] const std::string& get_last_path() const;

        [[nodiscard]] const std::string& get_last_string_params() const;

        [[nodiscard]] int64_t get_last_session_id() const;

    private:
        bool called_message_header_ = false;
        bool called_url_params_ = false;
        bool called_string_params_ = false;
        bool called_offline_ = false;

        message_header last_message_header_;
        std::string last_path_;
        std::string last_string_params_;
        int64_t last_session_id_ = 0;
    };
}