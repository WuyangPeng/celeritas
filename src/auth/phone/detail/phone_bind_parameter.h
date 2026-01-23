#pragma once

#include "phone_operation_parameter.h"
#include "auth/phone/phone_bind_response.h"

namespace celeritas
{
    class phone_bind_parameter final : public phone_operation_parameter
    {
    public:
        using class_type = phone_bind_parameter;
        using base_type = phone_operation_parameter;

        explicit phone_bind_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] phone_bind_response get_response() const;

        [[nodiscard]] std::string get_token() const;

    private:
        using optional_phone_bind_response = std::optional<phone_bind_response>;

        void init();

        optional_phone_bind_response response_;
        std::string token_;
    };
}