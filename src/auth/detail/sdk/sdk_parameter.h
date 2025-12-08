#pragma once

#include "auth/auth_fwd.h"
#include "auth/detail/core/auth_parameter.h"
#include "database/generated/mysql/auth/apps.h"

namespace celeritas
{
    class sdk_parameter : public auth_parameter
    {
    public:
        using class_type = sdk_parameter;
        using base_type = auth_parameter;

        explicit sdk_parameter(const http_handle_parameter& http_handle_parameter);

        [[nodiscard]] std::string get_sdk_token() const;

        [[nodiscard]] sdk_process_type get_process_type() const;

        [[nodiscard]] apps get_apps() const;

        [[nodiscard]] optional_http_response get_http_parameter() override;

    private:
        using optional_apps = std::optional<apps>;

        std::string sdk_token_;
        sdk_process_type process_type_;
        optional_apps apps_;
    };
}