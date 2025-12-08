#pragma once

#include "phone_parameter.h"
#include "auth/authentication/phone/send_sms_response.h"

namespace celeritas {
    class send_sms_parameter final : public phone_parameter {
    public:
        using class_type = send_sms_parameter;
        using base_type = phone_parameter;

        explicit send_sms_parameter(const http_handle_parameter &http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] send_sms_response get_response() const;

    private:
        using optional_send_sms_response = std::optional<send_sms_response>;

        void init();

        optional_send_sms_response response_;
    };
}