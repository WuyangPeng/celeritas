#include "resource_loader.h"

celeritas::resource_loader::resource_loader(const app_config& app_config)
    : app_config_{ app_config }
{
}

void celeritas::resource_loader::initialize()
{
    initialize_logger_resource();
    initialize_database_resource();
    initialize_server_resource();
    initialize_health_check_url_resource();
    initialize_service_registry_resource();
    service_initialize_resource();
}

void celeritas::resource_loader::initialize_logger_resource()
{
}

void celeritas::resource_loader::initialize_database_resource()
{
}

void celeritas::resource_loader::initialize_server_resource()
{
}

void celeritas::resource_loader::initialize_health_check_url_resource()
{
}

void celeritas::resource_loader::initialize_service_registry_resource()
{
}