#include "initializer.h"
#include "common/celeritas_error.h"
#include "server/server_fwd.h"
#include "service_registry_server/service_registry_initializer.h"

celeritas::initializer::initializer(std::string config_file_path) noexcept
    : config_file_path_{ std::move(config_file_path) }
{
}

void celeritas::initializer::initialize()
{
    initialize_config();
    initialize_resource();
    initialize_application();
}

celeritas::initializer::initializer_unique_ptr celeritas::initializer::create_initializer(const std::string& server_type, const std::string& config_file_path)
{
    if (server_type == service_registry_type)
    {
        return std::make_unique<service_registry_initializer>(config_file_path);
    }

    throw celeritas_error("unrecognized server type");
}