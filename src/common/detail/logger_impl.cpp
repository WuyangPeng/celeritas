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

using namespace std::literals;

auto get_formatter(bool is_console)
{
    // 设置日志格式
    return boost::log::expressions::stream
           << "["
           << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
           << "]["
           << boost::log::trivial::severity
           << "]["
           << boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID")
           << "]["
           << boost::log::expressions::attr<std::string>("file")
           << ":"
           << boost::log::expressions::attr<uint_least32_t>("line")
           << "]"
           << boost::log::expressions::smessage;
}

celeritas::logger_impl::logger_impl()
{
    default_logger_.add_attribute(channel.data(), boost::log::attributes::constant(default_channel));
    unregistered_logger_.add_attribute(channel.data(), boost::log::attributes::constant(unregistered_channel));
}

void celeritas::logger_impl::init_global(severity_level_type level)
{
    std::lock_guard lock{ mutex_ };

    // 添加通用属性，如时间戳
    boost::log::add_common_attributes();

    // 设置全局日志级别
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= level);
}

void celeritas::logger_impl::init_console(const severity_level_type console_level)
{
    std::lock_guard lock{ mutex_ };

    // 添加控制台日志输出
    if (console_sink_ == nullptr)
    {
        console_sink_ = boost::log::add_console_log(std::clog);
        console_sink_->set_formatter(get_formatter(true));
    }

    console_level_ = console_level;

    update_console_filter();
}

void celeritas::logger_impl::init_file(const std::string_view& channel_name, const std::string_view& log_file_name, severity_level_type file_level, int rotation_size, const bool also_to_console)
{
    std::lock_guard lock{ mutex_ };

    register_logger(channel_name);

    const auto current_path = boost::filesystem::current_path();

    const auto target_path = current_path / logger_path;

    if (!boost::filesystem::exists(target_path))
    {
        boost::filesystem::create_directories(target_path);
    }

    const auto file_pattern_part = log_file_name.data() + "_%Y%m%d_%N"s + logger_extension.data();

    const auto full_path_pattern = target_path / file_pattern_part;

    // 每天 00:00:00 轮换
    auto daily_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0);

    // 添加文件日志输出
    const auto file_sink = boost::log::add_file_log(
        boost::log::keywords::file_name = full_path_pattern.string(),
        boost::log::keywords::auto_flush = true,
        boost::log::keywords::rotation_size = rotation_size * 1024 * 1024,
        boost::log::keywords::time_based_rotation = daily_rotation,
        boost::log::keywords::filter = boost::log::expressions::has_attr(channel.data()) &&
                                       boost::log::expressions::attr<std::string>(channel.data()) == std::string{ channel_name } &&
                                       boost::log::trivial::severity >= file_level);

    file_sink->set_formatter(get_formatter(false));

    if (also_to_console)
    {
        if (console_channels_.insert(channel_name.data()).second)
        {
            update_console_filter();
        }
    }
    else
    {
        if (0 < console_channels_.erase(channel_name.data()))
        {
            update_console_filter();
        }
    }
}

celeritas::logger_impl::severity_logger_type& celeritas::logger_impl::get(const std::string_view& channel_name)
{
    if (channel_name == default_channel)
    {
        return default_logger_;
    }

    if (channel_name == unregistered_channel)
    {
        return unregistered_logger_;
    }

    std::shared_lock lock{ mutex_ };

    const auto iter = loggers_.find(channel_name.data());
    if (iter == loggers_.end())
    {
        BOOST_LOG_SEV(unregistered_logger_, boost::log::trivial::severity_level::warning) << "Logger channel not registered: " << channel_name;
        return unregistered_logger_;
    }
    return iter->second;
}

celeritas::logger_impl::severity_logger_type& celeritas::logger_impl::get_default() noexcept
{
    return default_logger_;
}

void celeritas::logger_impl::register_logger(const std::string_view& channel_name)
{
    const auto key = std::string{ channel_name };
    if (const auto iter = loggers_.find(key);
        iter == loggers_.end())
    {
        loggers_.emplace(key, severity_logger_type{});
        loggers_.at(key).add_attribute(channel.data(), boost::log::attributes::constant(std::string{channel_name}));
    }
}

void celeritas::logger_impl::update_console_filter()
{
    if (!console_sink_)
    {
        return;
    }

    auto console_filter = boost::log::trivial::severity >= console_level_;

    auto channel_filter = boost::log::expressions::has_attr(channel.data()) &&
                          boost::log::expressions::attr<std::string>(channel.data()) == "";
    for (const auto& element : console_channels_)
    {
        channel_filter = channel_filter ||
                         (boost::log::expressions::has_attr(element.data()) &&
                          boost::log::expressions::attr<std::string>(element.data()) == element);
    }
    console_filter = console_filter && (channel_filter || !boost::log::expressions::has_attr(channel.data()));

    console_sink_->set_filter(console_filter);
}
