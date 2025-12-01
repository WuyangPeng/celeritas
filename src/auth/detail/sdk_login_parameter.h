#pragma once

#include "sdk_parameter.h"
#include "auth/sdk_login_response.h"

namespace celeritas
{
    class sdk_login_parameter final : public sdk_parameter
    {
    public:
        using class_type = sdk_login_parameter;
        using base_type = sdk_parameter;

        explicit sdk_login_parameter(const http_handle_parameter& http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] sdk_login_response get_response() const;

    private:
        using optional_sdk_login_response = std::optional<sdk_login_response>;

        void init();

        optional_sdk_login_response response_;
    };
}