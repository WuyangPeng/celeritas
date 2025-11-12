#include "tools.h"
#include "common/logger.h"
#include "server/server_fwd.h"

#include <exception>

celeritas::tools::tools()
    : base_type{ tools_type }
{
}

void celeritas::tools::create_initializer(const command_line_config& command_line_config) const
{
}

int main(const int argc, char** argv)
{
    try
    {
        const celeritas::tools tools{};

        tools.run(argc, argv);
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

