#pragma once

#include "auth/core/auth_bind.h"

namespace celeritas
{
    class password_bind final : public auth_bind
    {
    public:
        using class_type = password_bind;
        using base_type = auth_bind;

        explicit password_bind(http_handle_parameter_shared_ptr handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

        [[nodiscard]] void_awaitable_type send_error_response() override;
    };
}