
#include "common/logger.h"
#include "initializer/initializer.h"
#include "server/server_fwd.h"
#include "common/common_fwd.h"

#include <boost/asio/io_context.hpp>
#include <exception>

int main()
{
    try
    {
        boost::asio::io_context io_context{};

        const auto initializer = celeritas::initializer::create_initializer(celeritas::service_registry_type, celeritas::service_registry_type, io_context);
        initializer->initialize();

        initializer->run();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(celeritas::default_channel, error) << "error:" << error.what();
        return 1;
    }

    return 0;
}