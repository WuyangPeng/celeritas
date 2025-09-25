#include "service_registry_resource_loader.h"

celeritas::service_registry_resource_loader::service_registry_resource_loader(const app_config& app_config)
    : base_type{ app_config }
{
}

void celeritas::service_registry_resource_loader::service_initialize_resource()
{
}