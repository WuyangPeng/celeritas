#include "service_registry_configuration_loader.h"

celeritas::service_registry_configuration_loader::service_registry_configuration_loader(const std::string_view config_file_path)
    : base_type{ config_file_path }
{
}

void celeritas::service_registry_configuration_loader::service_initialize_config()
{
}
