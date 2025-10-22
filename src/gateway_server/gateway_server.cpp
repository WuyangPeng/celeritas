#include "gateway_server.h"
#include "common/logger.h"
#include "server/server_fwd.h"

#include <exception>

celeritas::gateway_server::gateway_server()
    : base_type{ gateway_type }
{
}

int main(const int argc, char** argv)
{
    try
    {
        celeritas::gateway_server gateway_server{};

        gateway_server.run(argc, argv);
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