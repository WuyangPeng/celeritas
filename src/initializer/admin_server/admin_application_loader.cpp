#include "admin_application_loader.h"
#include "handler/generated/service_registry_response_message_handler.h"
#include "message/concrete_message_handler.tpp"
#include "service_registry/service_registry.h"
#include "service_registry/message_handler/discover_response_message_handler.h"
#include "service_registry/message_handler/register_response_message_handler.h"

celeritas::admin_application_loader::admin_application_loader(app_config_shared_ptr app_config)
    : base_type{ std::move(app_config) }
{
}

void celeritas::admin_application_loader::service_initialize_application()
{
    register_message_handler();
}

void celeritas::admin_application_loader::register_message_handler()
{
    register_handler(std::make_shared<service_registry_response_message_handler>());
    register_handler(std::make_shared<register_response_message_handler>());
    register_handler(std::make_shared<discover_response_message_handler>());
}