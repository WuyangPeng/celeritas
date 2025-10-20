#include "game_configuration_loader.h"

celeritas::game_configuration_loader::game_configuration_loader(std::string config_file_path)
    : base_type{ std::move(config_file_path) }
{
}

void celeritas::game_configuration_loader::service_initialize_config()
{
}
