#include "common/celeritas_error.h"
#include "logger_impl.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

auto get_formatter()
{
    // 设置日志格式
    return boost::log::expressions::stream
           << "["
           << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
           << "]"
           << " ["
           << boost::log::trivial::severity
           << "] "
           << boost::log::expressions::smessage;
}

celeritas::logger_impl::logger_impl()
{
    default_logger.add_attribute("Channel", boost::log::attributes::constant(default_channel));
}

void celeritas::logger_impl::init_global(severity_level_type level)
{
    // 添加通用属性，如时间戳
    boost::log::add_common_attributes();

    // 设置全局日志级别
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= level);
}

void celeritas::logger_impl::init_console(const severity_level_type level)
{
    // 添加控制台日志输出
    console_sink = boost::log::add_console_log(std::clog);
    console_level = level;
    console_sink->set_formatter(get_formatter());

    update_console_filter();
}

void celeritas::logger_impl::init_file(const std::string& channel_name, const std::string& log_file_name, severity_level_type file_level, const bool also_to_console)
{
    register_logger(channel_name);

    // 添加文件日志输出
    boost::log::add_file_log(
        boost::log::keywords::file_name = log_file_name,
        boost::log::keywords::auto_flush = true,
        boost::log::keywords::rotation_size = 10 * 1024 * 1024,
        boost::log::keywords::filter = boost::log::expressions::has_attr("Channel") &&
                                       boost::log::expressions::attr<std::string>("Channel") == channel_name &&
                                       boost::log::trivial::severity >= file_level)
        ->set_formatter(get_formatter());

    if (also_to_console)
    {
        console_channels.insert(channel_name);
    }
    else
    {
        console_channels.erase(channel_name);
    }

    update_console_filter();
}

celeritas::logger_impl::severity_logger_type& celeritas::logger_impl::get(const std::string& channel_name)
{
    if (channel_name == default_channel)
    {
        return default_logger;
    }

    const auto iter = loggers.find(channel_name);
    if (iter == loggers.end())
    {
        throw celeritas_error("Logger channel not registered: " + channel_name);
    }
    return iter->second;
}
celeritas::logger_impl::severity_logger_type& celeritas::logger_impl::get_default()
{
    return default_logger;
}

void celeritas::logger_impl::register_logger(const std::string& channel_name)
{
    if (const auto iter = loggers.find(channel_name);
        iter == loggers.end())
    {
        loggers.emplace(channel_name, severity_logger_type{});
        loggers.at(channel_name).add_attribute("Channel", boost::log::attributes::constant(channel_name));
    }
}

void celeritas::logger_impl::update_console_filter()
{
    if (!console_sink)
    {
        return;
    }

    auto console_filter = boost::log::trivial::severity >= console_level;

    auto channel_filter = boost::log::expressions::has_attr("Channel") && boost::log::expressions::attr<std::string>("Channel") == "";
    for (const auto& channel : console_channels)
    {
        channel_filter = channel_filter || (boost::log::expressions::has_attr("Channel") && boost::log::expressions::attr<std::string>("Channel") == channel);
    }
    console_filter = console_filter && (channel_filter || !boost::log::expressions::has_attr("Channel"));

    console_sink->set_filter(console_filter);
}
