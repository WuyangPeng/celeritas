#include "daemon.h"

#include <boost/filesystem/operations.hpp>
#include <unistd.h>
#include <filesystem>
#include <process.h>
#include <fstream>
#include <iostream>

celeritas::daemon::daemon(const std::string_view& server_type)
    : server_type_{ server_type }, pid_file_name_{ (boost::filesystem::current_path() / (server_type_ + ".txt")).string() }
{
    const auto pid = std::filesystem::path::preferred_separator == '\\' ? _getpid() : getpid();

    std::cout << pid_file_name_ << '\n';

    std::ofstream process_id{ pid_file_name_ };
    process_id << pid << std::endl;
}

celeritas::daemon::~daemon()
{
    std::filesystem::remove(pid_file_name_);
}