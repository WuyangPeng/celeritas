#include "service_registry_configuration_loader.h"

celeritas::service_registry_configuration_loader::service_registry_configuration_loader(std::string config_file_path)
    : base_type{ std::move(config_file_path) }
{
}

void celeritas::service_registry_configuration_loader::service_initialize_config()
{
}
