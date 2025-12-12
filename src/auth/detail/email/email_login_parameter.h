#pragma once

#include "email_operation_parameter.h"
#include "auth/authentication/email/email_login_response.h"

namespace celeritas
{
    class email_login_parameter final : public email_operation_parameter
    {
    public:
        using class_type = email_login_parameter;
        using base_type = email_operation_parameter;

        explicit email_login_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] email_login_response get_response() const;

    private:
        using optional_email_login_response = std::optional<email_login_response>;

        void init();

        optional_email_login_response response_;
    };
}