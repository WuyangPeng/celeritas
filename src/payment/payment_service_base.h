#pragma once

#include "message/http_handle_parameter.h"
#include "message/http_response.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class payment_service_base
    {
    public:
        using class_type = payment_service_base;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using io_context_type = boost::asio::io_context;

        explicit payment_service_base(http_handle_parameter handle_parameter);

        virtual ~payment_service_base() noexcept = default;

        payment_service_base(const payment_service_base& rhs) = default;

        payment_service_base& operator=(const payment_service_base& rhs) = delete;

        payment_service_base(payment_service_base&& rhs) noexcept = default;

        payment_service_base& operator=(payment_service_base&& rhs) noexcept = delete;

        [[nodiscard]] virtual void_awaitable_type response() = 0;

        void write(const http_response& response) const;

    protected:
        using optional_string = std::optional<std::string>;
        using app_config_const_shared_ptr = std::shared_ptr<const app_config>;

        [[nodiscard]] static std::string generate_token();

        [[nodiscard]] optional_string get_param(const std::string& key) const;

        [[nodiscard]] app_config_const_shared_ptr get_app_config() const;

        [[nodiscard]] const http_handle_parameter& get_http_handle_parameter() const;

        [[nodiscard]] io_context_type& get_io_context() const;

    private:
        http_handle_parameter handle_parameter_;
    };
}
