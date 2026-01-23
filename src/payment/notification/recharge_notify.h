#pragma once

#include "payment/config/app_sdk_payment_providers.h"
#include "message/message_fwd.h"

#include <memory>

namespace celeritas
{
    class recharge_notify
    {
    public:
        using class_type = recharge_notify;
        using recharge_notify_unique_ptr = std::unique_ptr<class_type>;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using http_handle_parameter_shared_ptr = std::shared_ptr<http_handle_parameter>;

        explicit recharge_notify(sdk_payment_providers sdk_payment_providers, http_handle_parameter_shared_ptr handle_parameter);

        virtual ~recharge_notify() noexcept = default;

        recharge_notify(const recharge_notify& rhs) noexcept = default;

        recharge_notify& operator=(const recharge_notify& rhs) noexcept = delete;

        recharge_notify(recharge_notify&& rhs) noexcept = default;

        recharge_notify& operator=(recharge_notify&& rhs) noexcept = delete;

        [[nodiscard]] virtual void_awaitable_type execute() const = 0;

        [[nodiscard]] virtual std::string get_default_message() const = 0;

        [[nodiscard]] static recharge_notify_unique_ptr create(sdk_payment_providers_key sdk_payment_providers_key, http_handle_parameter_shared_ptr handle_parameter);

    private:
        sdk_payment_providers sdk_payment_providers_;
        http_handle_parameter_shared_ptr handle_parameter_;
    };
}