#include "payment_configuration_loader.h"

celeritas::payment_configuration_loader::payment_configuration_loader(std::string config_file_path)
    : base_type{ std::move(config_file_path) }
{
}

void celeritas::payment_configuration_loader::service_initialize_config()
{
}
