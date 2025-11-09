#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>

#include <optional>
#include <shared_mutex>
#include <unordered_set>

namespace celeritas
{
    namespace log_sinks = boost::log::sinks;
    namespace log_sources = boost::log::sources;
    namespace log_trivial = boost::log::trivial;
    namespace log_keywords = boost::log::keywords;
    namespace log_attributes = boost::log::attributes;
    namespace log_expressions = boost::log::expressions;

    class logger_impl
    {
    public:
        using class_type = logger_impl;
        using severity_level_type = log_trivial::severity_level;
        using severity_logger_type = log_sources::severity_logger<severity_level_type>;
        using severity_logger_reference_type = std::reference_wrapper<severity_logger_type>;
        using severity_logger_optional_type = std::optional<severity_logger_reference_type>;

        logger_impl();

        ~logger_impl() noexcept = default;

        logger_impl(const logger_impl& rhs) = delete;

        logger_impl& operator=(const logger_impl& rhs) = delete;

        logger_impl(logger_impl&& rhs) noexcept = delete;

        logger_impl& operator=(logger_impl&& rhs) noexcept = delete;

        void init_global(severity_level_type level);

        void init_console(severity_level_type console_level);

        void init_file(const std::string& channel_name,
                       const std::string& log_file_name,
                       severity_level_type file_level,
                       int rotation_size,
                       bool also_to_console);

        [[nodiscard]] severity_logger_optional_type get(std::string_view channel_name, severity_level_type level);

        [[nodiscard]] severity_logger_optional_type get_default(severity_level_type level);

    private:
        using loggers_type = std::unordered_map<std::string, severity_logger_type>;
        using console_channels_type = std::unordered_set<std::string>;
        using console_sink_type = log_sinks::synchronous_sink<log_sinks::text_ostream_backend>;
        using console_sink_type_shared_ptr = boost::shared_ptr<console_sink_type>;
        using filesystem_path_type = boost::filesystem::path;
        using channel_levels_type = std::unordered_map<std::string, severity_level_type>;

        void register_logger(const std::string& channel_name);

        void update_console_filter();

        void update_console_filter(const std::string& channel_name, bool also_to_console);

        [[nodiscard]] static filesystem_path_type get_full_path_pattern(const std::string& log_file_name);

        loggers_type loggers_;
        console_channels_type console_channels_;
        console_sink_type_shared_ptr console_sink_;
        severity_level_type global_level_{};
        severity_level_type console_level_{};
        severity_logger_type default_logger_;
        severity_logger_type unregistered_logger_;
        channel_levels_type channel_levels_;
        std::shared_mutex mutex_;
    };
}
