#include "service_registry_application_loader.h"
#include "service_registry/message_handler/discover_request_message_handler.h"
#include "service_registry/message_handler/register_request_message_handler.h"
#include "service_registry/message_handler/service_registry_request_message_handler.h"

celeritas::service_registry_application_loader::service_registry_application_loader(app_config_shared_ptr app_config)
    : base_type{ std::move(app_config) }
{
}

void celeritas::service_registry_application_loader::service_initialize_application()
{
    registerHandler(std::make_shared<service_registry_request_message_handler>());
    registerHandler(std::make_shared<register_request_message_handler>());
    registerHandler(std::make_shared<discover_request_message_handler>());
}