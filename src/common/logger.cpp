#include "logger.h"
#include "detail/logger_impl.h"

void celeritas::logger::init_file(const std::string_view& channel_name, const std::string_view& log_file_name, const severity_level_type file_level, bool also_to_console)
{
    get_logger_impl().init_file(channel_name, log_file_name, file_level, also_to_console);
}

celeritas::logger::severity_logger_type& celeritas::logger::get(const std::string_view& channel_name)
{
    return get_logger_impl().get(channel_name);
}

celeritas::logger::severity_logger_type& celeritas::logger::get()
{
    return get_logger_impl().get("default");
}

celeritas::logger_impl& celeritas::logger::get_logger_impl()
{
    static logger_impl logger_impl{};

    return logger_impl;
}

void celeritas::logger::init_global(severity_level_type level)
{
    logger_impl::init_global(level);
}

void celeritas::logger::init_console(const severity_level_type console_level)
{
    get_logger_impl().init_console(console_level);
}
