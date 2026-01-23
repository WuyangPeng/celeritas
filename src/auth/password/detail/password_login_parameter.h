#pragma once

#include "password_parameter.h"
#include "auth/password/password_login_response.h"

namespace celeritas
{
    class password_login_parameter final : public password_parameter
    {
    public:
        using class_type = password_login_parameter;
        using base_type = password_parameter;

        explicit password_login_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] password_login_response get_response() const;

    private:
        using optional_password_login_response = std::optional<password_login_response>;

        void init();

        optional_password_login_response response_;
    };
}