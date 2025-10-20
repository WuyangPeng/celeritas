#include "chat_configuration_loader.h"

celeritas::chat_configuration_loader::chat_configuration_loader(std::string config_file_path)
    : base_type{ std::move(config_file_path) }
{
}

void celeritas::chat_configuration_loader::service_initialize_config()
{
}
