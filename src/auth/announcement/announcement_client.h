#pragma once

#include "message/basic/http_service_base.h"

namespace celeritas
{
    class announcement_client final : public http_service_base
    {
    public:
        using class_type = announcement_client;
        using base_type = http_service_base;

        explicit announcement_client(http_handle_parameter_shared_ptr handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

        [[nodiscard]] void_awaitable_type send_error_response() override;
    };
}
