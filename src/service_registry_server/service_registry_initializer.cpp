#include "service_registry_initializer.h"

celeritas::service_registry_initializer::service_registry_initializer(std::string_view config_file_path, boost::asio::io_context& io_context) noexcept
    : base_type{ config_file_path, io_context }
{
}

void celeritas::service_registry_initializer::service_initialize_config()
{
}

void celeritas::service_registry_initializer::initialize_resource()
{
}

void celeritas::service_registry_initializer::initialize_application()
{
}