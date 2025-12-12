#include "payment_platform_type.h"
#include "refund_notify.h"
#include "we_chat_refund_notify.h"
#include "common/celeritas_error.h"

celeritas::refund_notify::refund_notify(sdk_payment_providers sdk_payment_providers, http_handle_parameter_shared_ptr handle_parameter)
    : sdk_payment_providers_{ std::move(sdk_payment_providers) }, handle_parameter_{ std::move(handle_parameter) }
{
}

celeritas::refund_notify::refund_notify_unique_ptr celeritas::refund_notify::create(const sdk_payment_providers_key sdk_payment_providers_key, http_handle_parameter_shared_ptr handle_parameter)
{
    switch (auto sdk_payment_providers = app_sdk_payment_providers::get_instance().get_sdk_payment_providers(sdk_payment_providers_key);
        static_cast<payment_platform_type>(sdk_payment_providers.get_platform()))
    {
        case payment_platform_type::we_chat:
        {
            return std::make_unique<we_chat_refund_notify>(std::move(sdk_payment_providers), std::move(handle_parameter));
        }
        default:
        {
            throw celeritas_error("unsupported platform.");
        }
    }
}