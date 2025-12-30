#include "logger_container.h"
#include "common/common_fwd.h"

#include <boost/log/attributes/constant.hpp>

celeritas::logger_container::logger_container()
    : loggers_{}, default_logger_{}, unregistered_logger_{}
{
    default_logger_.add_attribute(channel.data(), log_attributes::constant(std::string{ default_channel }));
    unregistered_logger_.add_attribute(channel.data(), log_attributes::constant(std::string{ unregistered_channel }));
}

celeritas::logger_container::severity_logger_type& celeritas::logger_container::get(const std::string& channel_name)
{
    if (channel_name == default_channel)
    {
        return default_logger_;
    }

    if (channel_name == unregistered_channel)
    {
        return unregistered_logger_;
    }

    const auto iter = loggers_.find(channel_name);
    if (iter == loggers_.cend())
    {
        BOOST_LOG_SEV(unregistered_logger_, log_trivial::severity_level::warning) << "Logger channel not registered: " << channel_name;
        return unregistered_logger_;
    }

    return iter->second;
}

void celeritas::logger_container::register_logger(const std::string& channel_name)
{
    if (channel_name == default_channel || channel_name == unregistered_channel)
    {
        return;
    }

    if (const auto iter = loggers_.find(channel_name);
        iter == loggers_.cend())
    {
        severity_logger_type logger{};
        logger.add_attribute(channel.data(), log_attributes::constant(channel_name));

        loggers_.emplace(channel_name, std::move(logger));
    }
}