#include "logger_impl.h"
#include "logger_support.h"

#include <boost/filesystem/operations.hpp>
#include <boost/log/utility/setup/file.hpp>

void celeritas::logger_impl::init_global(severity_level_type global_level)
{
    std::lock_guard lock{ mutex_ };

    // 添加通用属性，如时间戳
    boost::log::add_common_attributes();

    // 设置全局日志级别
    boost::log::core::get()->set_filter(log_trivial::severity >= global_level);

    level_.set_global_level(global_level);
}

void celeritas::logger_impl::init_console(const severity_level_type console_level)
{
    std::lock_guard lock{ mutex_ };

    // 添加控制台日志输出
    console_.init_console();

    console_.update_console_filter(console_level);

    level_.set_console_level(console_level);
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

    loggers_.register_logger(channel_name);

    // 添加文件日志输出
    boost::log::add_file_log(
            log_keywords::file_name = full_path_pattern.string(),
            log_keywords::auto_flush = true,
            log_keywords::rotation_size = rotation_size * one_mebibyte,
            log_keywords::time_based_rotation = daily_rotation,
            log_keywords::open_mode = std::ios::app,
            log_keywords::scan_method = log_sinks::file::scan_method::scan_matching,
            log_keywords::filter = log_expressions::has_attr(channel.data()) &&
                                   log_expressions::attr<std::string>(channel.data()) == channel_name &&
                                   log_trivial::severity >= file_level)
        ->set_formatter(logger_support::get_formatter());

    console_.update_console_filter(level_.get_console_level(), channel_name, also_to_console);

    level_.set_channel_level(channel_name, file_level, also_to_console);
}

celeritas::logger_impl::severity_logger_optional_type celeritas::logger_impl::get(const std::string_view channel_name, const severity_level_type level)
{
    std::shared_lock lock{ mutex_ };

    if (!level_.can_logger(channel_name.data(), level))
    {
        return std::nullopt;
    }

    return loggers_.get(channel_name.data());
}

celeritas::logger_impl::severity_logger_optional_type celeritas::logger_impl::get_default(const severity_level_type level)
{
    return get(default_channel, level);
}

celeritas::logger_impl::filesystem_path_type celeritas::logger_impl::get_full_path_pattern(const std::string& log_file_name)
{
    const auto current_path = boost::filesystem::current_path();

    const auto target_path = current_path / logger_path;

    if (!boost::filesystem::exists(target_path))
    {
        boost::filesystem::create_directories(target_path);
    }

    const auto file_pattern_part = log_file_name + log_daily_suffix.data() + logger_extension.data();

    return target_path / file_pattern_part;
}
