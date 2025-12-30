#pragma once

#include "logger_namespace.h"

namespace celeritas
{
    class logger_level_data
    {
    public:
        using class_type = logger_level_data;

        logger_level_data() noexcept;

        logger_level_data(bool also_to_console, severity_level_type file_level);

        [[nodiscard]] bool is_also_to_console() const;

        [[nodiscard]] severity_level_type get_file_level() const;

        void set_also_to_console(bool also_to_console);

        void set_file_level(severity_level_type file_level);

    private:
        bool also_to_console_;
        severity_level_type file_level_;
    };
}