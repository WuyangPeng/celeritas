#include "payment_resource_loader.h"
#include "payment/app_sdk_payment_providers.h"

celeritas::payment_resource_loader::payment_resource_loader(const std::string_view server_type, app_config_shared_ptr app_config)
    : base_type{ server_type, std::move(app_config) }
{
}

void celeritas::payment_resource_loader::service_initialize_resource(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback)
{
    app_sdk_payment_providers::get_instance().load_from_db(any_io_executor);
}