#pragma once

#include "message/http_handle_parameter.h"
#include "message/http_response.h"

namespace celeritas
{
    class payment_parameter
    {
    public:
        using class_type = payment_parameter;
        using http_handle_parameter_shared_ptr = std::shared_ptr<http_handle_parameter>;

        explicit payment_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        virtual ~payment_parameter() noexcept = default;

        payment_parameter(const payment_parameter& rhs) = default;

        payment_parameter& operator=(const payment_parameter& rhs) = delete;

        payment_parameter(payment_parameter&& rhs) noexcept = default;

        payment_parameter& operator=(payment_parameter&& rhs) noexcept = delete;

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
        http_handle_parameter_shared_ptr http_handle_parameter_;
        int64_t app_id_;
        std::string sign_;
        int64_t timestamp_;
    };
}