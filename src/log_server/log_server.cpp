#include "log_server.h"
#include "common/logger/logger.h"
#include "initializer/initializer_fwd.h"

#include <exception>

celeritas::log_server::log_server()
    : base_type{ log_type }
{
}

int main(const int argc, char** argv)
{
    try
    {
        const celeritas::log_server server{};

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