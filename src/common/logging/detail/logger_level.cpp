#include "logger_level.h"

#include <ranges>

celeritas::severity_level_type celeritas::logger_level::get_console_level() const noexcept
{
    return console_level_;
}

bool celeritas::logger_level::can_logger(const std::string& channel_name, const severity_level_type level) const
{
    if (const auto iter = channel_levels_.find(channel_name);
        iter != channel_levels_.cend() && level < iter->second.get_channel_level())
    {
        return false;
    }

    return true;
}

void celeritas::logger_level::set_global_level(const severity_level_type global_level)
{
    global_level_ = global_level;

    for (auto& element : channel_levels_ | std::views::values)
    {
        element.set_channel_level(std::max(element.get_file_level(console_level_), global_level_));
    }
}

void celeritas::logger_level::set_console_level(const severity_level_type console_level)
{
    console_level_ = console_level;

    for (auto& element : channel_levels_ | std::views::values)
    {
        if (element.is_also_to_console())
        {
            element.set_channel_level(std::max(element.get_file_level(console_level_), global_level_));
        }
    }
}

void celeritas::logger_level::set_channel_level(const std::string& channel_name, const severity_level_type file_level, const bool also_to_console)
{
    auto& data = channel_levels_[channel_name];

    data.set_also_to_console(also_to_console);
    data.set_file_level(file_level);

    const auto channel_level = std::max(data.get_file_level(console_level_), global_level_);
    data.set_channel_level(channel_level);
}