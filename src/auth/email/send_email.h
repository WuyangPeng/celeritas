#pragma once

#include "auth/core/auth_service_base.h"
#include "database/generated/mysql/auth/apps.h"
#include "database/generated/redis/auth/email_code.h"
#include "message/parameters/http_handle_parameter.h"

namespace celeritas
{
    class send_email final : public auth_service_base
    {
    public:
        using class_type = send_email;
        using base_type = auth_service_base;

        explicit send_email(http_handle_parameter_shared_ptr handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

    private:
        [[nodiscard]] static void_awaitable_type send_sdk_sms(const email_code& sms_code, const apps& apps);
    };
}