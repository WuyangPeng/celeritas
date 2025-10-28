#include "daemon.h"

#include <boost/filesystem/operations.hpp>

#include <filesystem>
#include <fstream>

#ifdef BOOST_MSVC

#include <process.h>
#define GET_PID _getpid()

#else // !BOOST_MSVC

#include <unistd.h>
#define GET_PID getpid()

#endif // BOOST_MSVC

celeritas::daemon::daemon(const std::string_view server_type)
    : server_type_{ server_type }, pid_file_name_{ (boost::filesystem::current_path() / (server_type_ + ".pid")).string() }
{
    const auto pid = GET_PID;

    std::ofstream process_id{ pid_file_name_ };
    process_id << pid << std::endl;
}

celeritas::daemon::~daemon() noexcept
{
    std::filesystem::remove(pid_file_name_);
}