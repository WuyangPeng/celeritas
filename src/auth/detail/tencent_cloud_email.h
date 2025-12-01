#pragma once

#include "send_email_to_providers.h"

namespace celeritas
{
    class tencent_cloud_email final : public send_email_to_providers
    {
    public:
        using class_type = tencent_cloud_email;
        using base_type = send_email_to_providers;

        tencent_cloud_email(email_code email_code, email_providers email_providers);

        [[nodiscard]] void_awaitable_type execute() override;
    };
}