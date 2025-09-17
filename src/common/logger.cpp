#include "Logger.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;

// 初始化静态成员变量
src::severity_logger<celeritas::severity_level> celeritas::logger::logger_;

void celeritas::logger::Init(const std::string& log_file_name)
{
    // 设置日志格式
    logging::formatter formatter = expr::stream
        << "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") << "]"
        << " [" << logging::trivial::severity << "] "
        << expr::smessage;

    // 添加控制台日志输出
    auto console_sink = logging::add_console_log(std::clog);
    console_sink->set_formatter(formatter);

    // 添加文件日志输出
    logging::add_file_log(
        keywords::file_name = log_file_name,
        keywords::auto_flush = true,
        keywords::rotation_size = 10 * 1024 * 1024 // 10MB
    )->set_formatter(formatter);

    // 添加通用属性，如时间戳
    logging::add_common_attributes();

    // 设置全局日志级别
    logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
}

src::severity_logger<celeritas::severity_level>& celeritas::logger::get()
{
    return logger_;
}
