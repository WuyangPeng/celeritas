#include "gateway_resource_loader.h"

celeritas::gateway_resource_loader::gateway_resource_loader(app_config_shared_ptr app_config)
    : base_type{ std::move(app_config) }
{
}

void celeritas::gateway_resource_loader::service_initialize_resource()
{
}