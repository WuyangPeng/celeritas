#include "initializer_factory.h"
#include "common/celeritas_error.h"
#include "server/server_fwd.h"
#include "service_registry_server/service_registry_configuration_loader.h"

celeritas::initializer_factory::configuration_loader_unique_ptr celeritas::initializer_factory::create_configuration_loader(const std::string_view& server_type, const std::string_view& config_file_path)
{
    if (server_type == service_registry_type)
    {
        return std::make_unique<service_registry_configuration_loader>(config_file_path);
    }

    throw celeritas_error("unrecognized server type");
}