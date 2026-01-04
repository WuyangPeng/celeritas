#include "auth_server.h"
#include "common/logging/logger.h"
#include "initializer/initializer_constant.h"

#include <exception>

celeritas::auth_server::auth_server()
    : base_type{ auth_type }
{
}

int main(const int argc, char** argv)
{
    try
    {
        celeritas::auth_server server{};

        server.run(argc, argv);
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