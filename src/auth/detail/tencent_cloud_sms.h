#pragma once

#include "send_sms_to_providers.h"

namespace celeritas
{
    class tencent_cloud_sms final : public send_sms_to_providers
    {
    public:
        using class_type = tencent_cloud_sms;
        using base_type = send_sms_to_providers;

        tencent_cloud_sms(sms_code sms_code, sms_providers sms_providers);

        [[nodiscard]] void_awaitable_type execute() override;
    };
}