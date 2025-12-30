#include "logger_level.h"

celeritas::severity_level_type celeritas::logger_level::get_console_level() const
{
    return console_level_;
}

bool celeritas::logger_level::can_logger(const std::string& channel_name, const severity_level_type level) const
{
    if (const auto iter = channel_levels_.find(channel_name);
        iter != channel_levels_.cend())
    {
        const auto channel_level = std::max(std::min(console_level_, iter->second.get_file_level()), global_level_);

        return channel_level <= level;
    }

    return true;
}

void celeritas::logger_level::set_global_level(const severity_level_type global_level)
{
    global_level_ = global_level;
}

void celeritas::logger_level::set_console_level(const severity_level_type console_level)
{
    console_level_ = console_level;
}

void celeritas::logger_level::set_channel_level(const std::string& channel_name, const severity_level_type file_level, const bool also_to_console)
{
    channel_levels_[channel_name] = logger_level_data{ also_to_console, file_level };
}