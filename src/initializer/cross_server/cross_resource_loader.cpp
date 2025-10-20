#include "cross_resource_loader.h"

celeritas::cross_resource_loader::cross_resource_loader(app_config_shared_ptr app_config)
    : base_type{ std::move(app_config) }
{
}

void celeritas::cross_resource_loader::service_initialize_resource()
{
}