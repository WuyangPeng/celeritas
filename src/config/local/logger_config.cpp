#include "logger_config.h"
#include "common/core/celeritas_error.h"

#include <map>

celeritas::logger_config::logger_config(std::string name,
                                        const severity_level_type severity_level,
                                        const bool console_enabled,
                                        std::string channel_name,
                                        std::string log_file_name,
                                        const int rotation_size) noexcept
    : name_{ std::move(name) },
      level_{ severity_level },
      console_enabled_{ console_enabled },
      channel_name_{ std::move(channel_name) },
      log_file_name_{ std::move(log_file_name) },
      rotation_size_{ rotation_size }
{
}

std::string celeritas::logger_config::get_name() const
{
    return name_;
}

std::string celeritas::logger_config::get_channel_name() const
{
    return channel_name_;
}

std::string celeritas::logger_config::get_log_file_name() const
{
    return log_file_name_;
}

celeritas::severity_level_type celeritas::logger_config::get_severity_level_type() const noexcept
{
    return level_;
}

int celeritas::logger_config::get_rotation_size() const noexcept
{
    return rotation_size_;
}

bool celeritas::logger_config::is_console_enabled() const noexcept
{
    return console_enabled_;
}

celeritas::severity_level_type celeritas::logger_config::get_severity_level_type(const std::string& severity_level_name)
{
    using severity_level_container_type = std::map<std::string, severity_level_type>;

    static severity_level_container_type severity_level{ { "trace", log_trivial::trace },
                                                         { "debug", log_trivial::debug },
                                                         { "info", log_trivial::info },
                                                         { "warning", log_trivial::warning },
                                                         { "error", log_trivial::error },
                                                         { "fatal", log_trivial::fatal } };

    if (const auto iter = severity_level.find(severity_level_name);
        iter != severity_level.cend())
    {
        return iter->second;
    }

    throw celeritas_error{ "get_severity_level_type error,name ={}", severity_level_name };
}