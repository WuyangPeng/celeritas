#pragma once

#include "auth_service_base.h"

namespace celeritas
{
    class login_servers final : public auth_service_base
    {
    public:
        using class_type = login_servers;
        using base_type = auth_service_base;

        explicit login_servers(http_handle_parameter handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;
    };
}