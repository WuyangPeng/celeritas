#include "payment_application_loader.h"
#include "handler/payment/manual/notify_http_message_handler.h"
#include "handler/payment/manual/refund_http_message_handler.h"
#include "message/handler_base/concrete_message_handler.tpp"
#include "../../payment/config/app_sdk_payment_providers.h"

celeritas::payment_application_loader::payment_application_loader(const std::string_view server_type, app_config_shared_ptr app_config)
    : base_type{ server_type, std::move(app_config) }
{
}

void celeritas::payment_application_loader::service_initialize_application()
{
    register_message_handler();
}

void celeritas::payment_application_loader::register_message_handler()
{
    for (const auto sdk_payment_providers = app_sdk_payment_providers::get_instance().get_sdk_payment_providers();
         const auto& sdk_payment_provider : sdk_payment_providers | std::views::values)
    {
        const sdk_payment_providers_key sdk_payment_providers_key{ sdk_payment_provider.get_app_id(), static_cast<payment_platform_type>(sdk_payment_provider.get_platform()) };

        register_handler(std::make_shared<notify_http_message_handler>(sdk_payment_providers_key, sdk_payment_provider.get_http_suffix()));
        register_handler(std::make_shared<refund_http_message_handler>(sdk_payment_providers_key, sdk_payment_provider.get_http_suffix()));
    }
}