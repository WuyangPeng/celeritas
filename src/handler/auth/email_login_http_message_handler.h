#pragma once

#include "message/handler_base/http_base_message_handler.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class email_login_http_message_handler final : public http_base_message_handler
    {
    public:
        using class_type = email_login_http_message_handler;
        using base_type = http_base_message_handler;

        email_login_http_message_handler() noexcept = default;

        [[nodiscard]] std::string get_supported_type_name() const override;

        [[nodiscard]] bool handle(const http_handle_parameter_shared_ptr& handle_parameter, const http_message_registry_weak_ptr& message_registry) override;

    private:
        using void_awaitable_type = boost::asio::awaitable<void>;

        [[nodiscard]] static void_awaitable_type response(http_handle_parameter_shared_ptr handle_parameter);
    };
}