#include "gateway_configuration_loader.h"

celeritas::gateway_configuration_loader::gateway_configuration_loader(std::string config_file_path)
    : base_type{ std::move(config_file_path) }
{
}

void celeritas::gateway_configuration_loader::service_initialize_config()
{
}
