#pragma once

#include "password_parameter.h"
#include "auth/password_bind_response.h"

namespace celeritas
{
    class password_bind_parameter final : public password_parameter
    {
    public:
        using class_type = password_bind_parameter;
        using base_type = password_parameter;

        explicit password_bind_parameter(const http_handle_parameter& http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] password_bind_response get_response() const;

        [[nodiscard]] std::string get_token() const;

    private:
        using optional_password_bind_response = std::optional<password_bind_response>;

        void init();

        optional_password_bind_response response_;
        std::string token_;
    };
}