#pragma once

#include <boost/log/trivial.hpp>

namespace celeritas
{
    class logger_level_config
    {
    public:
        using class_type = logger_level_config;
        using severity_level_type = boost::log::trivial::severity_level;

    private:
        severity_level_type default_level_ = severity_level_type::trace;
        severity_level_type console_level_ = severity_level_type::trace;
    };
}
