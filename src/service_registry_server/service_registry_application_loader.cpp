#include "service_registry_application_loader.h"

celeritas::service_registry_application_loader::service_registry_application_loader(const app_config_shared_ptr& app_config)
    : base_type{ app_config }
{
}

void celeritas::service_registry_application_loader::service_initialize_application()
{
}