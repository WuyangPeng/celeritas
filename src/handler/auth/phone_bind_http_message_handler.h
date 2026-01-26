#pragma once

#include "message/handler_base/http_base_message_handler.h"

namespace celeritas
{
    class phone_bind_http_message_handler final : public http_base_message_handler
    {
    public:
        using class_type = phone_bind_http_message_handler;
        using base_type = http_base_message_handler;

        phone_bind_http_message_handler() noexcept = default;

        [[nodiscard]] std::string get_supported_type_name() const override;

        [[nodiscard]] bool handle(const http_handle_parameter_shared_ptr& handle_parameter, const http_message_registry_weak_ptr& message_registry) override;

        [[nodiscard]] std::string get_server_type() const override;
    };
}