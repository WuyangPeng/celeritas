#pragma once

#include "message/basic/http_response.h"
#include "message/parameters/http_handle_parameter.h"

namespace celeritas
{
    class http_parameter
    {
    public:
        using class_type = http_parameter;
        using http_handle_parameter_shared_ptr = std::shared_ptr<http_handle_parameter>;

        explicit http_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter);

        virtual ~http_parameter() noexcept = default;

        http_parameter(const http_parameter& rhs) = default;

        http_parameter& operator=(const http_parameter& rhs) = delete;

        http_parameter(http_parameter&& rhs) noexcept = default;

        http_parameter& operator=(http_parameter&& rhs) noexcept = delete;

        [[nodiscard]] int64_t get_app_id() const;

        [[nodiscard]] std::string get_sign() const;

        [[nodiscard]] int64_t get_timestamp() const;

        [[nodiscard]] virtual bool is_failure() const = 0;

    protected:
        using optional_http_response = std::optional<http_response>;
        using optional_string = std::optional<std::string>;

        [[nodiscard]] virtual optional_http_response get_http_parameter();

        [[nodiscard]] optional_string get_param(const std::string& key) const;

        [[nodiscard]] optional_string get_param(std::string_view key) const;

        [[nodiscard]] optional_string get_param(const char* key) const;

    private:
        http_handle_parameter_shared_ptr http_handle_parameter_;
        int64_t app_id_;
        std::string sign_;
        int64_t timestamp_;
    };
}
