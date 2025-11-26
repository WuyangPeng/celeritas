#pragma once

#include "config_fwd.h"
#include "common/detail/logger_namespace.h"

#include <boost/property_tree/ptree.hpp>

namespace celeritas
{
    class logger_config
    {
    public:
        using class_type = logger_config;

        logger_config() noexcept = default;

        logger_config(std::string name, severity_level_type severity_level, bool console_enabled, std::string channel_name, std::string log_file_name, int rotation_size);

        [[nodiscard]] std::string get_name() const;

        [[nodiscard]] std::string get_channel_name() const;

        [[nodiscard]] std::string get_log_file_name() const;

        [[nodiscard]] severity_level_type get_severity_level_type() const;

        [[nodiscard]] int get_rotation_size() const;

        [[nodiscard]] bool is_console_enabled() const;

        [[nodiscard]] static severity_level_type get_severity_level_type(const std::string& severity_level_name);

    private:
        std::string name_;
        severity_level_type level_ = severity_level_type::trace;
        bool console_enabled_ = false;
        std::string channel_name_;
        std::string log_file_name_;
        int rotation_size_ = default_logger_rotation_size;
    };
}
