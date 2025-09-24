#include "app_config.h"
#include "detail/service_registry_config_reader.h"


void celeritas::app_config::load_service_registry_config(const std::string& filename)
{
    service_registry_ = service_registry_config_reader::load_config(filename);
}