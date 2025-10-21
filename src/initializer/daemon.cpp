#include "daemon.h"

#include <boost/filesystem/operations.hpp>
#include <unistd.h>
#include <filesystem>
#include <fstream>

celeritas::daemon::daemon(const std::string_view& server_type)
    : server_type_{ server_type }
{
    const auto pid = std::filesystem::path::preferred_separator == '\\' ? _getpid() : getpid();

    std::ofstream process_id{ boost::filesystem::current_path().string() + server_type_ };
    process_id << pid << std::endl;
}

celeritas::daemon::~daemon()
{
    std::filesystem::remove(server_type_);
}