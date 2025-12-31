#include "logic_server.h"
#include "common/logging/logger.h"
#include "initializer/initializer_fwd.h"

#include <exception>

celeritas::logic_server::logic_server()
    : base_type{ logic_type }
{
}

int main(const int argc, char** argv)
{
    try
    {
        celeritas::logic_server server{};

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