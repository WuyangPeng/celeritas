#include "logic_configuration_loader.h"

celeritas::logic_configuration_loader::logic_configuration_loader(std::string config_file_path)
    : base_type{ std::move(config_file_path) }
{
}

void celeritas::logic_configuration_loader::service_initialize_config()
{
}
