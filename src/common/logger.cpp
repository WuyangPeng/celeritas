#include "logger.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/setup/console.hpp>

boost::log::sources::severity_logger<celeritas::severity_level> celeritas::logger::logger_;

void celeritas::logger::init(const std::string& log_file_name)
{
    // 设置日志格式
    const auto formatter = boost::log::expressions::stream
                           << "["
                           << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
                           << "]"
                           << " ["
                           << boost::log::trivial::severity
                           << "] "
                           << boost::log::expressions::smessage;

    // 添加控制台日志输出
    const auto console_sink = boost::log::add_console_log(std::clog);
    console_sink->set_formatter(formatter);

    // 添加文件日志输出
    boost::log::add_file_log(
        boost::log::keywords::file_name = log_file_name,
        boost::log::keywords::auto_flush = true,
        boost::log::keywords::rotation_size = 10 * 1024 * 1024 // 10MB
        )->set_formatter(formatter);

    // 添加通用属性，如时间戳
    boost::log::add_common_attributes();

    // 设置全局日志级别
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
}

boost::log::sources::severity_logger<celeritas::severity_level>& celeritas::logger::get()
{
    return logger_;
}
