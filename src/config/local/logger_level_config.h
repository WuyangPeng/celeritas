#pragma once

#include "common/logging/detail/logger_namespace.h"

#include <optional>

namespace celeritas
{
    class logger_level_config
    {
    public:
        using class_type = logger_level_config;

        [[nodiscard]] bool is_set_default_level() const noexcept;

        [[nodiscard]] bool is_set_console_level() const noexcept;

        [[nodiscard ]] severity_level_type get_default_level() const noexcept;

        [[nodiscard ]] severity_level_type get_console_level() const noexcept;

        void set_default_level(severity_level_type severity_level_type) noexcept;

        void set_console_level(severity_level_type severity_level_type) noexcept;

    private:
        using optional_severity_level_type = std::optional<severity_level_type>;

        optional_severity_level_type default_level_;
        optional_severity_level_type console_level_;
    };
}
