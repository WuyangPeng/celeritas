#pragma once

#include "logger_namespace.h"

#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>

#include <string>
#include <unordered_set>

namespace celeritas
{
    class logger_console
    {
    public:
        using class_type = logger_console;

        void init_console();

        void update_console_filter(severity_level_type console_level) const;

        void update_console_filter(severity_level_type console_level, const std::string& channel_name, bool also_to_console);

    private:
        using console_channels_type = std::unordered_set<std::string>;
        using console_sink_type = log_sinks::synchronous_sink<log_sinks::text_ostream_backend>;
        using console_sink_type_shared_ptr = boost::shared_ptr<console_sink_type>;

        console_channels_type console_channels_;
        console_sink_type_shared_ptr console_sink_;
    };
}