#pragma once

#include "logger_namespace.h"

#include <string>
#include <unordered_map>

namespace celeritas
{
    class logger_container
    {
    public:
        using class_type = logger_container;
        using severity_logger_type = log_sources::severity_logger<severity_level_type>;

        logger_container();

        [[nodiscard]] severity_logger_type& get(const std::string& channel_name);

        void register_logger(const std::string& channel_name);

    private:
        using loggers_type = std::unordered_map<std::string, severity_logger_type>;

        loggers_type loggers_;
        severity_logger_type default_logger_;
        severity_logger_type unregistered_logger_;
    };
}