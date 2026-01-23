#pragma once

#include "auth/core/detail/auth_parameter.h"
#include "database/generated/mysql/auth/apps.h"

namespace celeritas
{
    class phone_parameter : public auth_parameter
    {
    public:
        using class_type = phone_parameter;
        using base_type = auth_parameter;

        explicit phone_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        [[nodiscard]] std::string get_phone() const;

        [[nodiscard]] apps get_apps() const;

        [[nodiscard]] optional_http_response get_http_parameter() override;

    private:
        using optional_apps = std::optional<apps>;

        std::string phone_;
        optional_apps apps_;
    };
}