#pragma once

#include "message/http_base_message_handler.h"
#include "payment/sdk_payment_providers_key.h"

#include <boost/asio/awaitable.hpp>

namespace celeritas
{
    class refund_http_message_handler final : public http_base_message_handler
    {
    public:
        using class_type = refund_http_message_handler;
        using base_type = http_base_message_handler;

        explicit refund_http_message_handler(const sdk_payment_providers_key& sdk_payment_providers_key, std::string path_suffix);

        [[nodiscard]] std::string get_supported_type_name() const override;

        [[nodiscard]] bool handle(const http_handle_parameter_shared_ptr& handle_parameter, const http_message_registry_weak_ptr& message_registry) override;

    private:
        using void_awaitable_type = boost::asio::awaitable<void>;

        [[nodiscard]] static void_awaitable_type response(sdk_payment_providers_key sdk_payment_providers_key, http_handle_parameter_shared_ptr handle_parameter);

        sdk_payment_providers_key sdk_payment_providers_key_;
        std::string path_suffix_;
    };
}