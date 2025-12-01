#pragma once

#include "auth/auth_fwd.h"
#include "database/generated/mysql/auth/sdk_providers.h"

namespace celeritas
{
    class sdk_process_parameter
    {
    public:
        using class_type = sdk_process_parameter;

        sdk_process_parameter(std::string sdk_token, sdk_providers sdk_providers);

        [[nodiscard]] std::string get_sdk_token() const;

        [[nodiscard]] sdk_providers get_sdk_providers() const;

        [[nodiscard]] sdk_process_type get_process_type() const;

    private:
        std::string sdk_token_;
        sdk_providers sdk_providers_;
    };
}