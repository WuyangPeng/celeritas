#pragma once

#include <boost/log/trivial.hpp>

namespace celeritas
{
    class logger_level_config
    {
    public:
        using class_type = logger_level_config;
        using severity_level_type = boost::log::trivial::severity_level;

        [[nodiscard]] bool is_set_default_level() const noexcept;

        [[nodiscard]] bool is_set_console_level() const noexcept;

        [[nodiscard ]] severity_level_type get_default_level() const noexcept;

        [[nodiscard ]] severity_level_type get_console_level() const noexcept;

        void set_default_level(severity_level_type severity_level_type);

        void set_console_level(severity_level_type severity_level_type);

    private:
        severity_level_type default_level_ = severity_level_type::trace;
        bool is_set_default_level_ = false;
        severity_level_type console_level_ = severity_level_type::trace;
        bool is_set_console_level_ = false;
    };
}
