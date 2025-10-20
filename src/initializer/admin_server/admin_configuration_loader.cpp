#include "admin_configuration_loader.h"

celeritas::admin_configuration_loader::admin_configuration_loader(std::string config_file_path)
    : base_type{ std::move(config_file_path) }
{
}

void celeritas::admin_configuration_loader::service_initialize_config()
{
}
