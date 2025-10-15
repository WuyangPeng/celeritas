#include "initializer_factory.h"
#include "common/celeritas_error.h"
#include "server/server_fwd.h"
#include "service_registry_server/service_registry_application_loader.h"
#include "service_registry_server/service_registry_configuration_loader.h"
#include "service_registry_server/service_registry_resource_loader.h"

celeritas::initializer_factory::configuration_loader_unique_ptr celeritas::initializer_factory::create_configuration_loader(const std::string_view& server_type, std::string config_file_path)
{
    if (server_type == service_registry_type)
    {
        return std::make_unique<service_registry_configuration_loader>(config_file_path);
    }

    throw celeritas_error("unrecognized server type");
}

celeritas::initializer_factory::resource_loader_unique_ptr celeritas::initializer_factory::create_resource_loader(const std::string_view& server_type, const app_config_shared_ptr& app_config)
{
    if (server_type == service_registry_type)
    {
        return std::make_unique<service_registry_resource_loader>(app_config);
    }

    throw celeritas_error("unrecognized server type");
}

celeritas::initializer_factory::application_loader_unique_ptr celeritas::initializer_factory::create_application_loader(const std::string_view& server_type, const app_config_shared_ptr& app_config)
{
    if (server_type == service_registry_type)
    {
        return std::make_unique<service_registry_application_loader>(app_config);
    }

    throw celeritas_error("unrecognized server type");
}