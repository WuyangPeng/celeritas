#include "daemon.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "common/logging/logger.h"

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
    noexcept_safe_call_and_log([this] {
                                   this->stop();
                               },
                               initializer_channel,
                               "daemon stop error: ");
}

void celeritas::daemon::stop()
{
    std::error_code error_code{};
    std::filesystem::remove(pid_file_name_, error_code);

    if (error_code)
    {
        LOG_CHANNEL(initializer_channel, warning)
        << "⚠️ 警告：无法删除 PID 文件 \"" << pid_file_name_ << "\". "
        << "错误信息: " << error_code.message()
        << " (代码: " << error_code.value()
        << ", 类别: " << error_code.category().name() << ")";
    }
}