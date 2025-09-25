#include "logger_config.h"

std::string celeritas::logger_config::get_name() const
{
    return name_;
}

std::string celeritas::logger_config::get_channel_name() const
{
    return channel_name_;
}