#include "cross_configuration_loader.h"

celeritas::cross_configuration_loader::cross_configuration_loader(std::string config_file_path)
    : base_type{ std::move(config_file_path) }
{
}

void celeritas::cross_configuration_loader::service_initialize_config()
{
}
