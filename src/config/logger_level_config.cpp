#include "logger_level_config.h"

bool celeritas::logger_level_config::is_set_default_level() const noexcept
{
    return is_set_default_level_;
}

bool celeritas::logger_level_config::is_set_console_level() const noexcept
{
    return is_set_console_level_;
}

celeritas::logger_level_config::severity_level_type celeritas::logger_level_config::get_default_level() const noexcept
{
    return default_level_;
}

celeritas::logger_level_config::severity_level_type celeritas::logger_level_config::get_console_level() const noexcept
{
    return console_level_;
}

void celeritas::logger_level_config::set_default_level(const severity_level_type severity_level_type)
{
    default_level_ = severity_level_type;
    is_set_default_level_ = true;
}

void celeritas::logger_level_config::set_console_level(const severity_level_type severity_level_type)
{
    console_level_ = severity_level_type;
    is_set_console_level_ = true;
}