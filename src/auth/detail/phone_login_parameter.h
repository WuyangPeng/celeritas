#pragma once

#include "phone_operation_parameter.h"
#include "auth/phone_login_response.h"

namespace celeritas
{
    class phone_login_parameter final : public phone_operation_parameter
    {
    public:
        using class_type = phone_login_parameter;
        using base_type = phone_operation_parameter;

        explicit phone_login_parameter(const http_handle_parameter& http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] phone_login_response get_response() const;

    private:
        using optional_phone_login_response = std::optional<phone_login_response>;

        void init();

        optional_phone_login_response response_;
    };
}