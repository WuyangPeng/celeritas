#pragma once

#include "sdk_parameter.h"
#include "auth/authentication/sdk/sdk_bind_response.h"

namespace celeritas
{
    class sdk_bind_parameter final : public sdk_parameter
    {
    public:
        using class_type = sdk_bind_parameter;
        using base_type = sdk_parameter;

        explicit sdk_bind_parameter(const http_handle_parameter& http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] sdk_bind_response get_response() const;

        [[nodiscard]] std::string get_token() const;

    private:
        using optional_sdk_bind_response = std::optional<sdk_bind_response>;

        void init();

        optional_sdk_bind_response response_;
        std::string token_;
    };
}