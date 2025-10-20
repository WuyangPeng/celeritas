#include "auth_configuration_loader.h"

celeritas::auth_configuration_loader::auth_configuration_loader(std::string config_file_path)
    : base_type{ std::move(config_file_path) }
{
}

void celeritas::auth_configuration_loader::service_initialize_config()
{
}
