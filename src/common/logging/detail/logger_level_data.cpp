#include "logger_level_data.h"

celeritas::logger_level_data::logger_level_data() noexcept
    : also_to_console_{}, file_level_{}, channel_level_{}
{
}

celeritas::logger_level_data::logger_level_data(const bool also_to_console, const severity_level_type file_level, severity_level_type channel_level) noexcept
    : also_to_console_{ also_to_console }, file_level_{ file_level }, channel_level_{ channel_level }
{
}

bool celeritas::logger_level_data::is_also_to_console() const noexcept
{
    return also_to_console_;
}

celeritas::severity_level_type celeritas::logger_level_data::get_file_level() const noexcept
{
    return file_level_;
}

void celeritas::logger_level_data::set_also_to_console(const bool also_to_console) noexcept
{
    also_to_console_ = also_to_console;
}

void celeritas::logger_level_data::set_file_level(const severity_level_type file_level) noexcept
{
    file_level_ = file_level;
}

celeritas::severity_level_type celeritas::logger_level_data::get_channel_level() const noexcept
{
    return channel_level_;
}

void celeritas::logger_level_data::set_channel_level(const severity_level_type channel_level) noexcept
{
    channel_level_ = channel_level;
}

celeritas::severity_level_type celeritas::logger_level_data::get_file_level(const severity_level_type console_level) const
{
    if (also_to_console_)
    {
        return std::min(console_level, file_level_);
    }

    return file_level_;
}

