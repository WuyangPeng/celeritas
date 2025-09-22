#include "service_registry_initializer.h"

celeritas::service_registry_initializer::service_registry_initializer(std::string config_file_path) noexcept
    : base_type{ std::move(config_file_path) }
{
}

void celeritas::service_registry_initializer::initialize_config()
{
}

void celeritas::service_registry_initializer::initialize_resource()
{
}

void celeritas::service_registry_initializer::initialize_application()
{
}