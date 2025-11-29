#pragma once

#include "auth_parameter.h"
#include "auth/send_sms_response.h"
#include "database/generated/mysql/auth/apps.h"

namespace celeritas
{
    class send_sms_parameter final : public auth_parameter
    {
    public:
        using class_type = send_sms_parameter;
        using base_type = auth_parameter;

        explicit send_sms_parameter(const http_handle_parameter& http_handle_parameter);

        [[nodiscard]] std::string get_phone() const;

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] send_sms_response get_response() const;

        [[nodiscard]] apps get_apps() const;

    private:
        using optional_send_sms_response = std::optional<send_sms_response>;
        using optional_apps = std::optional<apps>;

        void init();

        optional_send_sms_response response_;
        std::string phone_;
        optional_apps apps_;
    };
}