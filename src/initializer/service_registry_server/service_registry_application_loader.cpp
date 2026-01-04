#include "service_registry_application_loader.h"
#include "message/handler_base/concrete_message_handler.tpp"

celeritas::service_registry_application_loader::service_registry_application_loader(app_config_shared_ptr app_config)
    : base_type{ std::move(app_config) }
{
}

void celeritas::service_registry_application_loader::service_initialize_application()
{
    register_message_handler();
}

void celeritas::service_registry_application_loader::register_message_handler()
{
}