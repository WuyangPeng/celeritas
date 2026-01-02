#include "logger_console.h"
#include "logger_support.h"

#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>

void celeritas::logger_console::init_console()
{
    if (console_sink_ == nullptr)
    {
        console_sink_ = boost::log::add_console_log(std::clog);
        console_sink_->set_formatter(logger_support::get_formatter());
    }
}

void celeritas::logger_console::update_console_filter(severity_level_type console_level) const
{
    if (!console_sink_)
    {
        return;
    }

    auto console_filter = log_trivial::severity >= console_level;

    auto channel_filter = log_expressions::has_attr(channel.data()) &&
                          log_expressions::attr<std::string>(channel.data()) == "";

    for (const auto& element : console_channels_)
    {
        channel_filter = channel_filter ||
                         (log_expressions::has_attr(element.data()) &&
                          log_expressions::attr<std::string>(element.data()) == element);
    }
    console_filter = console_filter && (channel_filter || !log_expressions::has_attr(channel.data()));

    console_sink_->set_filter(console_filter);
}

void celeritas::logger_console::update_console_filter(const severity_level_type console_level, const std::string& channel_name, const bool also_to_console)
{
    if (also_to_console)
    {
        if (!console_channels_.insert(channel_name).second)
        {
            return;
        }
    }
    else if (console_channels_.erase(channel_name) <= 0)
    {
        return;
    }

    update_console_filter(console_level);
}