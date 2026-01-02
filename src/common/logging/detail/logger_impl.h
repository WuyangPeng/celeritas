#pragma once

#include "logger_console.h"
#include "logger_container.h"
#include "logger_level.h"
#include "logger_namespace.h"

#include <boost/filesystem/path.hpp>

#include <optional>
#include <shared_mutex>

namespace celeritas
{
    class logger_impl
    {
    public:
        using class_type = logger_impl;
        using severity_logger_type = log_sources::severity_logger<severity_level_type>;
        using severity_logger_reference_type = std::reference_wrapper<severity_logger_type>;
        using severity_logger_optional_type = std::optional<severity_logger_reference_type>;

        logger_impl() noexcept = default;

        ~logger_impl() noexcept = default;

        logger_impl(const logger_impl& rhs) = delete;

        logger_impl& operator=(const logger_impl& rhs) = delete;

        logger_impl(logger_impl&& rhs) noexcept = delete;

        logger_impl& operator=(logger_impl&& rhs) noexcept = delete;

        void init_global(severity_level_type global_level);

        void init_console(severity_level_type console_level);

        void init_file(const std::string& channel_name,
                       const std::string& log_file_name,
                       severity_level_type file_level,
                       int rotation_size,
                       bool also_to_console);

        [[nodiscard]] severity_logger_optional_type get(std::string_view channel_name, severity_level_type level);

        [[nodiscard]] severity_logger_optional_type get_default(severity_level_type level);

    private:
        using filesystem_path_type = boost::filesystem::path;

        [[nodiscard]] static filesystem_path_type get_full_path_pattern(const std::string& log_file_name);

        logger_container loggers_;
        logger_console console_;
        logger_level level_;
        std::shared_mutex mutex_;
    };
}
