#include "logger.h"
#include "detail/logger_impl.h"

void celeritas::logger::init_global(const severity_level_type global_level)
{
    get_logger_impl().init_global(global_level);
}

void celeritas::logger::init_console(const severity_level_type console_level)
{
    get_logger_impl().init_console(console_level);
}

void celeritas::logger::init_file(const std::string& channel_name,
                                  const std::string& log_file_name,
                                  const severity_level_type file_level,
                                  const int rotation_size,
                                  const bool also_to_console)
{
    get_logger_impl().init_file(channel_name, log_file_name, file_level, rotation_size, also_to_console);
}

celeritas::logger::severity_logger_optional_type celeritas::logger::get(const std::string_view channel_name, const severity_level_type level)
{
    return get_logger_impl().get(channel_name, level);
}

celeritas::logger::severity_logger_optional_type celeritas::logger::get_default(const severity_level_type level)
{
    return get_logger_impl().get_default(level);
}

celeritas::logger_impl& celeritas::logger::get_logger_impl() noexcept
{
    static logger_impl logger_impl{};

    return logger_impl;
}
