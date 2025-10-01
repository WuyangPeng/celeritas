
#include "service_registry_server.h"
#include "common/common_fwd.h"
#include "common/logger.h"
#include "initializer/initializer.h"
#include "server/server_fwd.h"

#include <boost/asio/io_context.hpp>
#include <exception>

void celeritas::service_registry_server::run()
{
    boost::asio::io_context io_context{};

    const auto server_context = std::make_shared<initializer>(service_registry_type, service_registry_type, io_context);
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