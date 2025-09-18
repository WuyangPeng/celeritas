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
        void init_file(const std::string& channel_name, const std::string& log_file_name, severity_level_type file_level, bool also_to_console);

        [[nodiscard]] severity_logger_type& get(const std::string& channel_name);
        [[nodiscard]] severity_logger_type& get_default();

    private:
        void register_logger(const std::string& channel_name);
        void update_console_filter();

        std::unordered_map<std::string, severity_logger_type> loggers;
        std::unordered_set<std::string> console_channels;
        boost::shared_ptr<console_sink_type> console_sink;
        severity_level_type console_level{};
        std::string default_channel{ "default" };
        severity_logger_type default_logger;
    };
}
