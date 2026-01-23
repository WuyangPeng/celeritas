#pragma once

#include "recharge_notify.h"

namespace celeritas
{
    class we_chat_recharge_notify final : public recharge_notify
    {
    public:
        using class_type = we_chat_recharge_notify;
        using bass_type = recharge_notify;

        explicit we_chat_recharge_notify(sdk_payment_providers sdk_payment_providers, http_handle_parameter_shared_ptr handle_parameter);

        [[nodiscard]] void_awaitable_type execute() const override;

        [[nodiscard]] std::string get_default_message() const override;
    };
}
