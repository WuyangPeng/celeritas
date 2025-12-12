#pragma once

#include "app_sdk_payment_providers.h"
#include "message/http_handle_parameter.h"

#include <memory>

namespace celeritas
{
    class refund_notify
    {
    public:
        using class_type = refund_notify;
        using refund_notify_unique_ptr = std::unique_ptr<class_type>;
        using void_awaitable_type = boost::asio::awaitable<void>;
        using http_handle_parameter_shared_ptr = std::shared_ptr<http_handle_parameter>;

        explicit refund_notify(sdk_payment_providers sdk_payment_providers, http_handle_parameter_shared_ptr handle_parameter);

        virtual ~refund_notify() noexcept = default;

        refund_notify(const refund_notify& rhs) noexcept = default;

        refund_notify& operator=(const refund_notify& rhs) noexcept = delete;

        refund_notify(refund_notify&& rhs) noexcept = default;

        refund_notify& operator=(refund_notify&& rhs) noexcept = delete;

        [[nodiscard]] virtual void_awaitable_type execute() const = 0;

        [[nodiscard]] static refund_notify_unique_ptr create(sdk_payment_providers_key sdk_payment_providers_key, http_handle_parameter_shared_ptr handle_parameter);

    private:
        sdk_payment_providers sdk_payment_providers_;
        http_handle_parameter_shared_ptr handle_parameter_;
    };
}