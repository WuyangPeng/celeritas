#pragma once

#include "database/generated/mysql/auth/apps.h"
#include "database/generated/redis/auth/sms_code.h"
#include "message/basic/http_service_base.h"

namespace celeritas
{
    class send_sms final : public http_service_base
    {
    public:
        using class_type = send_sms;
        using base_type = http_service_base;

        explicit send_sms(http_handle_parameter_shared_ptr handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

        [[nodiscard]] void_awaitable_type send_error_response() override;

    private:
        [[nodiscard]] static void_awaitable_type send_sdk_sms(const sms_code& sms_code, const apps& apps);
    };
}