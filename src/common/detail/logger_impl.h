#pragma once

#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>

#include <unordered_set>

namespace celeritas
{
    class logger_impl
    {
    public:
        using class_type = logger_impl;
        using severity_level_type = boost::log::trivial::severity_level;
        using severity_logger_type = boost::log::sources::severity_logger<severity_level_type>;
        using console_sink_type = boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend>;

        logger_impl();

        static void init_global(severity_level_type level);
        void init_console(severity_level_type level);
        void init_file(const std::string_view& channel_name,
                       const std::string_view& log_file_name,
                       severity_level_type file_level,
                       bool also_to_console);

        [[nodiscard]] severity_logger_type& get(const std::string_view& channel_name);
        [[nodiscard]] severity_logger_type& get_default();

    private:
        using loggers_type = std::unordered_map<std::string, severity_logger_type>;
        using console_channels_type = std::unordered_set<std::string>;
        using console_sink_type_shared_ptr = boost::shared_ptr<console_sink_type>;

        void register_logger(const std::string_view& channel_name);
        void update_console_filter();

        loggers_type loggers_;
        console_channels_type console_channels_;
        console_sink_type_shared_ptr console_sink_;
        severity_level_type console_level_{};
        severity_logger_type default_logger_;
        severity_logger_type unregistered_logger_;
        std::mutex mutex_;
    };
}
