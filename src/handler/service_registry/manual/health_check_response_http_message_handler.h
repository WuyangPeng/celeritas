#pragma once

#include "message/http_base_message_handler.h"

namespace celeritas
{
    class health_check_response_http_message_handler final : public http_base_message_handler
    {
    public:
        using class_type = health_check_response_http_message_handler;
        using base_type = http_base_message_handler;

        explicit health_check_response_http_message_handler(std::string path);

        [[nodiscard]] std::string get_supported_type_name() const override;

        [[nodiscard]] bool handle(const http_handle_parameter_shared_ptr& handle_parameter, const http_message_registry_weak_ptr& message_registry) override;

    private:
        std::string path_;
    };
}