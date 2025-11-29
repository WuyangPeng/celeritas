#pragma once

#include "auth_service_base.h"
#include "database/database_pool_base.h"
#include "database/generated/mysql/auth/account.h"
#include "message/http_handle_parameter.h"

namespace celeritas
{
    class phone_bind final : public auth_service_base
    {
    public:
        using class_type = phone_bind;
        using base_type = auth_service_base;

        explicit phone_bind(http_handle_parameter handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;
    };
}