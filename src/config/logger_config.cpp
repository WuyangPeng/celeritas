#include "logger_config.h"
#include "common/celeritas_error.h"

celeritas::logger_config::logger_config(std::string name, severity_level_type severity_level, bool console_enabled, std::string channel_name, std::string log_file_name, int rotation_size)
    : name_{ std::move(name) }, level_{ severity_level }, console_enabled_{ console_enabled }, channel_name_{ std::move(channel_name) }, log_file_name_{ std::move(log_file_name) }, rotation_size_{ rotation_size }
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

celeritas::logger_config::severity_level_type celeritas::logger_config::get_severity_level_type() const
{
    return level_;
}

int celeritas::logger_config::get_rotation_size() const
{
    return rotation_size_;
}

bool celeritas::logger_config::is_console_enabled() const
{
    return console_enabled_;
}

celeritas::logger_config::severity_level_type celeritas::logger_config::get_severity_level_type(const std::string& severity_level_name)
{
    if (severity_level_name == "trace")
    {
        return boost::log::trivial::trace;
    }

    if (severity_level_name == "debug")
    {
        return boost::log::trivial::debug;
    }

    if (severity_level_name == "info")
    {
        return boost::log::trivial::info;
    }

    if (severity_level_name == "warning")
    {
        return boost::log::trivial::warning;
    }

    if (severity_level_name == "error")
    {
        return boost::log::trivial::error;
    }

    if (severity_level_name == "fatal")
    {
        return boost::log::trivial::fatal;
    }

    throw new celeritas_error("get_severity_level_type error,name =" + severity_level_name);
}