#include "payment_application_loader.h"

celeritas::payment_application_loader::payment_application_loader(app_config_shared_ptr app_config)
    : base_type{ std::move(app_config) }
{
}

void celeritas::payment_application_loader::service_initialize_application()
{
    register_message_handler();
}

void celeritas::payment_application_loader::register_message_handler()
{
}