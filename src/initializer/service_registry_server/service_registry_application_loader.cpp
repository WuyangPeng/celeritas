#include "service_registry_application_loader.h"
#include "service_registry/message_handler/discover_request_message_handler.h"
#include "service_registry/message_handler/register_request_message_handler.h"
#include "service_registry/message_handler/service_registry_request_message_handler.h"
#include "message/concrete_message_handler.tpp"

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
    register_handler(std::make_shared<service_registry_request_message_handler>());
    register_handler(std::make_shared<register_request_message_handler>());
    register_handler(std::make_shared<discover_request_message_handler>());
}