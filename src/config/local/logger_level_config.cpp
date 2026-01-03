#include "logger_level_config.h"

bool celeritas::logger_level_config::is_set_default_level() const noexcept
{
    return default_level_.has_value();
}

bool celeritas::logger_level_config::is_set_console_level() const noexcept
{
    return console_level_.has_value();
}

celeritas::severity_level_type celeritas::logger_level_config::get_default_level() const noexcept
{
    if (default_level_)
    {
        return *default_level_;
    }

    return boost::log::trivial::trace;
}

celeritas::severity_level_type celeritas::logger_level_config::get_console_level() const noexcept
{
    if (console_level_)
    {
        return *console_level_;
    }

    return boost::log::trivial::trace;
}

void celeritas::logger_level_config::set_default_level(const severity_level_type severity_level_type) noexcept
{
    default_level_ = severity_level_type;
}

void celeritas::logger_level_config::set_console_level(const severity_level_type severity_level_type) noexcept
{
    console_level_ = severity_level_type;
}