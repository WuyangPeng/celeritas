#pragma once

#include <boost/log/trivial.hpp>

namespace celeritas
{
    class logger_config
    {
    public:
        using class_type = logger_config;
        using severity_level_type = boost::log::trivial::severity_level;

        [[nodiscard]] std::string get_name() const;

        [[nodiscard]] std::string get_channel_name() const;

    private:
        std::string name_;
        severity_level_type level_ = severity_level_type::trace;
        bool console_enabled_ = false;
        std::string channel_name_;
        std::string log_file_name_;
        int rotation_size_ = 0;
    };
}
