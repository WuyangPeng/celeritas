#include "logger_impl.h"
#include "common/common_fwd.h"
#include "config/config_fwd.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace celeritas
{
    auto get_formatter()
    {
        // 设置日志格式
        return log_expressions::stream
               << "["
               << log_expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
               << "]["
               << log_trivial::severity
               << "]["
               << log_expressions::attr<log_attributes::current_thread_id::value_type>("ThreadID")
               << "]["
               << log_expressions::attr<std::string>("file")
               << ":"
               << log_expressions::attr<uint_least32_t>("line")
               << "]"
               << log_expressions::smessage;
    }
}

celeritas::logger_impl::logger_impl()
{
    default_logger_.add_attribute(channel.data(), log_attributes::constant(std::string{ default_channel }));
    unregistered_logger_.add_attribute(channel.data(), log_attributes::constant(std::string{ unregistered_channel }));
}

void celeritas::logger_impl::init_global(severity_level_type level)
{
    std::lock_guard lock{ mutex_ };

    // 添加通用属性，如时间戳
    boost::log::add_common_attributes();

    // 设置全局日志级别
    boost::log::core::get()->set_filter(log_trivial::severity >= level);
}

void celeritas::logger_impl::init_console(const severity_level_type console_level)
{
    std::lock_guard lock{ mutex_ };

    // 添加控制台日志输出
    if (console_sink_ == nullptr)
    {
        console_sink_ = boost::log::add_console_log(std::clog);
        console_sink_->set_formatter(get_formatter());
    }

    console_level_ = console_level;

    update_console_filter();
}

void celeritas::logger_impl::init_file(const std::string& channel_name,
                                       const std::string& log_file_name,
                                       severity_level_type file_level,
                                       const int rotation_size,
                                       const bool also_to_console)
{
    const auto full_path_pattern = get_full_path_pattern(log_file_name);

    // 每天 00:00:00 轮换
    auto daily_rotation = log_sinks::file::rotation_at_time_point(0, 0, 0);

    std::lock_guard lock{ mutex_ };

    register_logger(channel_name);

    // 添加文件日志输出
    boost::log::add_file_log(
            log_keywords::file_name = full_path_pattern.string(),
            log_keywords::auto_flush = true,
            log_keywords::rotation_size = rotation_size * 1024 * 1024,
            log_keywords::time_based_rotation = daily_rotation,
            log_keywords::open_mode = std::ios::app,
            log_keywords::scan_method = log_sinks::file::scan_method::scan_matching,
            log_keywords::filter = log_expressions::has_attr(channel.data()) &&
                                   log_expressions::attr<std::string>(channel.data()) == channel_name &&
                                   log_trivial::severity >= file_level)
        ->set_formatter(get_formatter());

    update_console_filter(channel_name, also_to_console);
}

celeritas::logger_impl::severity_logger_type& celeritas::logger_impl::get(std::string_view channel_name)
{
    if (channel_name == default_channel)
    {
        return default_logger_;
    }

    if (channel_name == unregistered_channel)
    {
        return unregistered_logger_;
    }

    std::lock_guard lock{ mutex_ };

    const auto iter = loggers_.find(channel_name.data());
    if (iter == loggers_.end())
    {
        BOOST_LOG_SEV(unregistered_logger_, log_trivial::severity_level::warning) << "Logger channel not registered: " << channel_name;
        return unregistered_logger_;
    }
    return iter->second;
}

celeritas::logger_impl::severity_logger_type& celeritas::logger_impl::get_default() noexcept
{
    return default_logger_;
}

void celeritas::logger_impl::register_logger(const std::string& channel_name)
{
    if (const auto iter = loggers_.find(channel_name);
        iter == loggers_.end())
    {
        loggers_.emplace(channel_name, severity_logger_type{});
        loggers_.at(channel_name).add_attribute(channel.data(), log_attributes::constant(channel_name));
    }
}

void celeritas::logger_impl::update_console_filter()
{
    if (!console_sink_)
    {
        return;
    }

    auto console_filter = log_trivial::severity >= console_level_;

    auto channel_filter = log_expressions::has_attr(channel.data()) &&
                          log_expressions::attr<std::string>(channel.data()) == "";
    for (const auto& element : console_channels_)
    {
        channel_filter = channel_filter ||
                         (log_expressions::has_attr(element.data()) &&
                          log_expressions::attr<std::string>(element.data()) == element);
    }
    console_filter = console_filter && (channel_filter || !log_expressions::has_attr(channel.data()));

    console_sink_->set_filter(console_filter);
}

void celeritas::logger_impl::update_console_filter(const std::string& channel_name, const bool also_to_console)
{
    if (also_to_console)
    {
        if (console_channels_.insert(channel_name).second)
        {
            update_console_filter();
        }
    }
    else
    {
        if (0 < console_channels_.erase(channel_name))
        {
            update_console_filter();
        }
    }
}

celeritas::logger_impl::filesystem_path_type celeritas::logger_impl::get_full_path_pattern(const std::string& log_file_name)
{
    const auto current_path = boost::filesystem::current_path();

    const auto target_path = current_path / logger_path;

    if (!boost::filesystem::exists(target_path))
    {
        boost::filesystem::create_directories(target_path);
    }

    const auto file_pattern_part = log_file_name + "_%Y%m%d_%N" + logger_extension.data();

    return target_path / file_pattern_part;
}
