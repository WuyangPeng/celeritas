#include "daemon.h"

#include <unistd.h>
#include <filesystem>
#include <fstream>

celeritas::daemon::daemon()
{
    const auto pid = std::filesystem::path::preferred_separator == '\\' ? _getpid() : getpid();

    std::ofstream process_id{ "process_id.txt" };
    process_id << pid << std::endl;
}

celeritas::daemon::~daemon()
{
    std::filesystem::remove("process_id.txt");
}