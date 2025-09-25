#include "service_registry_resource_loader.h"

celeritas::service_registry_resource_loader::service_registry_resource_loader(app_config app_config)
    : base_type{ std::move(app_config) }
{
}

void celeritas::service_registry_resource_loader::service_initialize_resource()
{
}