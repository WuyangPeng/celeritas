#pragma once

#include "boost/property_tree/ptree.hpp"

#include <boost/log/trivial.hpp>

namespace celeritas
{
    class logger_config
    {
    public:
        using class_type = logger_config;
        using severity_level_type = boost::log::trivial::severity_level;

        logger_config() noexcept = default;

        logger_config(std::string name, severity_level_type severity_level, bool console_enabled, std::string channel_name, std::string log_file_name, int rotation_size);

        [[nodiscard]] std::string get_name() const;

        [[nodiscard]] std::string get_channel_name() const;

        [[nodiscard]] static severity_level_type get_severity_level_type(const std::string& severity_level_name);

    private:
        std::string name_;
        severity_level_type level_ = severity_level_type::trace;
        bool console_enabled_ = false;
        std::string channel_name_;
        std::string log_file_name_;
        int rotation_size_ = 0;
    };
}
