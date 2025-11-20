#pragma once

#include "message/http_base_message_handler.h"

namespace celeritas
{
    class guest_login_http_message_handler final : public http_base_message_handler
    {
    public:
        using class_type = guest_login_http_message_handler;
        using base_type = http_base_message_handler;

        guest_login_http_message_handler() noexcept = default;

        [[nodiscard]] std::string get_supported_type_name() const override;

        [[nodiscard]] bool handle(const http_handle_parameter& handle_parameter, const http_message_registry_weak_ptr& message_registry) override;
    };
}
