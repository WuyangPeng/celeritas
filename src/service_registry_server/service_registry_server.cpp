#include "service_registry_server.h"
#include "common/common_fwd.h"
#include "common/logger.h"
#include "initializer/initializer.h"
#include "server/server_fwd.h"

#include <exception>

void celeritas::service_registry_server::run()
{
    const auto server_context = initializer::create(service_registry_type, service_registry_type);

    server_context->initialize();
    server_context->run();
}

int main()
{
    try
    {
        celeritas::service_registry_server::run();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(celeritas::default_channel, error) << "error:" << error.what();
        return 1;
    }
    catch (...)
    {
        LOG_CHANNEL(celeritas::default_channel, fatal) << "unknown error.";
        return 1;
    }

    return 0;
}