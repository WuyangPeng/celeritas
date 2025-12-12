#pragma once

#include "auth/detail/core/auth_parameter.h"
#include "database/generated/mysql/auth/apps.h"

namespace celeritas
{
    class email_parameter : public auth_parameter
    {
    public:
        using class_type = email_parameter;
        using base_type = auth_parameter;

        explicit email_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        [[nodiscard]] std::string get_email() const;

        [[nodiscard]] apps get_apps() const;

        [[nodiscard]] optional_http_response get_http_parameter() override;

    private:
        using optional_apps = std::optional<apps>;

        std::string email_;
        optional_apps apps_;
    };
}