#include "logger_container.h"
#include "common/common_constant.h"

#include <boost/log/attributes/constant.hpp>

celeritas::logger_container::logger_container()
    : loggers_{}, default_logger_{}, unregistered_logger_{}
{
    register_logger(default_channel.data());
    register_logger(unregistered_channel.data());
}

celeritas::logger_container::severity_logger_type& celeritas::logger_container::get(const std::string& channel_name)
{
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
    if (const auto iter = loggers_.find(channel_name);
        iter == loggers_.cend())
    {
        severity_logger_type logger{};
        logger.add_attribute(channel.data(), log_attributes::constant(channel_name));

        loggers_.emplace(channel_name, std::move(logger));
    }
}