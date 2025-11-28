#pragma once

#include "auth/guest_login_response.h"
#include "message/http_handle_parameter.h"

namespace celeritas
{
    class guest_login_parameter
    {
    public:
        using class_type = guest_login_parameter;

        explicit guest_login_parameter(const http_handle_parameter& http_handle_parameter);

        [[nodiscard]] std::string get_device_id() const;

        [[nodiscard]] int64_t get_app_id() const;

        [[nodiscard]] bool is_failure() const;

        [[nodiscard]] guest_login_response get_response() const;

    private:
        using optional_guest_login_response = std::optional<guest_login_response>;

        void init();

        const http_handle_parameter& http_handle_parameter_;
        optional_guest_login_response response_;
        std::string device_id_;
        int64_t app_id_;
        std::string sign_;
        int64_t timestamp_;
    };
}