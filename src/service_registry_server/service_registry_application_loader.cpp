#include "service_registry_application_loader.h"

celeritas::service_registry_application_loader::service_registry_application_loader(app_config_shared_ptr app_config)
    : base_type{ std::move(app_config) }
{
}

void celeritas::service_registry_application_loader::service_initialize_application()
{
}