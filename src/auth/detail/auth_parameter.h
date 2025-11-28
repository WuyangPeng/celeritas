#pragma once

#include "message/http_handle_parameter.h"
#include "message/http_response.h"

namespace celeritas
{
    class auth_parameter
    {
    public:
        using class_type = auth_parameter;

        explicit auth_parameter(const http_handle_parameter& http_handle_parameter);

        virtual ~auth_parameter() noexcept = default;

        auth_parameter(const auth_parameter& rhs) = default;

        auth_parameter& operator=(const auth_parameter& rhs) = delete;

        auth_parameter(auth_parameter&& rhs) noexcept = default;

        auth_parameter& operator=(auth_parameter&& rhs) noexcept = delete;

        [[nodiscard]] int64_t get_app_id() const;

        [[nodiscard]] std::string get_sign() const;

        [[nodiscard]] int64_t get_timestamp() const;

        [[nodiscard]] virtual bool is_failure() const = 0;

    protected:
        using optional_http_response = std::optional<http_response>;
        using optional_string = std::optional<std::string>;

        [[nodiscard]] virtual optional_http_response get_http_parameter();

        [[nodiscard]] optional_string get_param(const std::string& key) const;

    private:
        const http_handle_parameter& http_handle_parameter_;
        int64_t app_id_;
        std::string sign_;
        int64_t timestamp_;
    };
}