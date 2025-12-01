#include "auth_resource_loader.h"
#include "auth/app_email_providers.h"
#include "auth/app_sdk_providers.h"
#include "auth/app_secret.h"
#include "auth/app_sms_providers.h"

celeritas::auth_resource_loader::auth_resource_loader(const std::string_view server_type, app_config_shared_ptr app_config)
    : base_type{ server_type, std::move(app_config) }
{
}

void celeritas::auth_resource_loader::service_initialize_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
    app_secret::get_instance().load_from_db(io_context);
    app_sms_providers::get_instance().load_from_db(io_context);
    app_email_providers::get_instance().load_from_db(io_context);
    app_sdk_providers::get_instance().load_from_db(io_context);
}