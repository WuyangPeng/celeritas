#include "battle_configuration_loader.h"

celeritas::battle_configuration_loader::battle_configuration_loader(std::string config_file_path)
    : base_type{ std::move(config_file_path) }
{
}

void celeritas::battle_configuration_loader::service_initialize_config()
{
}
