#pragma once

#include "auth_service_base.h"
#include "database/generated/mysql/auth/apps.h"
#include "database/generated/redis/auth/sms_code.h"
#include "message/http_handle_parameter.h"

namespace celeritas
{
    class send_sms final : public auth_service_base
    {
    public:
        using class_type = send_sms;
        using base_type = auth_service_base;

        explicit send_sms(http_handle_parameter handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

    private:
        [[nodiscard]] static void_awaitable_type send_sdk_sms(const sms_code& sms_code, const apps& apps);
    };
}