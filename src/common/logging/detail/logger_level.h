#pragma once

#include "logger_level_data.h"
#include "logger_namespace.h"

namespace celeritas
{
    class logger_level
    {
    public:
        using class_type = logger_level;

        [[nodiscard]] severity_level_type get_console_level() const noexcept;

        [[nodiscard]] bool can_logger(const std::string& channel_name, severity_level_type level) const;

        void set_global_level(severity_level_type global_level);

        void set_console_level(severity_level_type console_level);

        void set_channel_level(const std::string& channel_name, severity_level_type file_level, bool also_to_console);

    private:
        using channel_levels_type = std::unordered_map<std::string, logger_level_data>;

        severity_level_type global_level_{};
        severity_level_type console_level_{};
        channel_levels_type channel_levels_;
    };
}