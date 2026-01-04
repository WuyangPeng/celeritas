#include "cross_server.h"
#include "common/logging/logger.h"
#include "initializer/initializer_constant.h"

#include <exception>

celeritas::cross_server::cross_server()
    : base_type{ cross_type }
{
}

int main(const int argc, char** argv)
{
    try
    {
        celeritas::cross_server server{};

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