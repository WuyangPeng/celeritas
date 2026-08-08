#pragma once

#include "message/basic/http_service_base.h"

namespace celeritas
{
    class feedback_submit final : public http_service_base
    {
    public:
        using class_type = feedback_submit;
        using base_type = http_service_base;

        explicit feedback_submit(http_handle_parameter_shared_ptr handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

        [[nodiscard]] void_awaitable_type send_error_response() override;
    };
}
