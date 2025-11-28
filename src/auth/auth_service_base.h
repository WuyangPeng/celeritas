#pragma once

#include "message/http_handle_parameter.h"
#include "message/http_response.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class auth_service_base
    {
    public:
        using class_type = auth_service_base;
        using void_awaitable_type = boost::asio::awaitable<void>;

        explicit auth_service_base(http_handle_parameter handle_parameter);

        virtual ~auth_service_base() noexcept = default;

        auth_service_base(const auth_service_base& rhs) = default;

        auth_service_base& operator=(const auth_service_base& rhs) = delete;

        auth_service_base(auth_service_base&& rhs) noexcept = default;

        auth_service_base& operator=(auth_service_base&& rhs) noexcept = delete;

        [[nodiscard]] virtual void_awaitable_type response() = 0;

    protected:
        using optional_string = std::optional<std::string>;
        using app_config_const_shared_ptr = std::shared_ptr<const app_config>;

        [[nodiscard]] static std::string generate_token();

        void write(const http_response& response) const;

        [[nodiscard]] optional_string get_param(const std::string& key) const;

        [[nodiscard]] app_config_const_shared_ptr get_app_config() const;

    private:
        http_handle_parameter handle_parameter_;
    };
}
