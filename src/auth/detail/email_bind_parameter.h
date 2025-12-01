#pragma once

#include "email_operation_parameter.h"
#include "auth/email_bind_response.h"

namespace celeritas
{
    class email_bind_parameter final : public email_operation_parameter
    {
    public:
        using class_type = email_bind_parameter;
        using base_type = email_operation_parameter;

        explicit email_bind_parameter(const http_handle_parameter& http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] email_bind_response get_response() const;

        [[nodiscard]] std::string get_token() const;

    private:
        using optional_email_bind_response = std::optional<email_bind_response>;

        void init();

        optional_email_bind_response response_;
        std::string token_;
    };
}