#pragma once

#include "refund_notify.h"

namespace celeritas
{
    class we_chat_refund_notify final : public refund_notify
    {
    public:
        using class_type = we_chat_refund_notify;
        using bass_type = refund_notify;

        explicit we_chat_refund_notify(sdk_payment_providers sdk_payment_providers, http_handle_parameter handle_parameter);

        [[nodiscard]] void_awaitable_type execute() const override;
    };
}
