#pragma once

#include "email_parameter.h"
#include "auth/authentication/email/send_email_response.h"

namespace celeritas {
    class send_email_parameter final : public email_parameter {
    public:
        using class_type = send_email_parameter;
        using base_type = email_parameter;

        explicit send_email_parameter(const http_handle_parameter &http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] send_email_response get_response() const;

    private:
        using optional_send_email_response = std::optional<send_email_response>;

        void init();

        optional_send_email_response response_;
    };
}