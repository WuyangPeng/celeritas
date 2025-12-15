#include "player_configuration_loader.h"

celeritas::player_configuration_loader::player_configuration_loader(std::string config_file_path)
    : base_type{ std::move(config_file_path) }
{
}

void celeritas::player_configuration_loader::service_initialize_config()
{
}