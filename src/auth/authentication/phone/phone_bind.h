#pragma once

#include "auth/core/auth_bind.h"
#include "message/http_handle_parameter.h"

namespace celeritas
{
    class phone_bind final : public auth_bind
    {
    public:
        using class_type = phone_bind;
        using base_type = auth_bind;

        explicit phone_bind(http_handle_parameter_shared_ptr handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;
    };
}