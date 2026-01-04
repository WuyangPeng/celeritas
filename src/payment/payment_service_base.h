#pragma once

#include "config/config_fwd.h"
#include "message/basic/http_response.h"
#include "message/message_fwd.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class payment_service_base
    {
    public:
        using class_type = payment_service_base;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using io_context_type = boost::asio::io_context;
        using any_io_executor = boost::asio::any_io_executor;
        using http_handle_parameter_shared_ptr = std::shared_ptr<http_handle_parameter>;

        explicit payment_service_base(http_handle_parameter_shared_ptr handle_parameter);

        virtual ~payment_service_base() noexcept = default;

        payment_service_base(const payment_service_base& rhs) = default;

        payment_service_base& operator=(const payment_service_base& rhs) = delete;

        payment_service_base(payment_service_base&& rhs) noexcept = default;

        payment_service_base& operator=(payment_service_base&& rhs) noexcept = delete;

        [[nodiscard]] virtual void_awaitable_type response() = 0;

        [[nodiscard]] void_awaitable_type write_immediately(const http_response& response) const;

    protected:
        using optional_string = std::optional<std::string>;
        using app_config_const_shared_ptr = std::shared_ptr<const app_config>;

        [[nodiscard]] static std::string generate_token();

        [[nodiscard]] optional_string get_param(const std::string& key) const;

        [[nodiscard]] app_config_const_shared_ptr get_app_config() const;

        [[nodiscard]] http_handle_parameter_shared_ptr get_http_handle_parameter() const;

        [[nodiscard]] any_io_executor get_any_io_executor() const;

    private:
        http_handle_parameter_shared_ptr handle_parameter_;
    };
}
