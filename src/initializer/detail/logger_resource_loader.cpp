#include "logger_resource_loader.h"
#include "common/logger.h"

void celeritas::logger_resource_loader::loader_level_config(const logger_level_config& logger_level_config)
{
    if (logger_level_config.is_set_default_level())
    {
        logger::init_global(logger_level_config.get_default_level());
    }

    if (logger_level_config.is_set_console_level())
    {
        logger::init_console(logger_level_config.get_console_level());
    }
}

void celeritas::logger_resource_loader::loader_logger(const logger_config& logger_config)
{
    logger::init_file(logger_config.get_channel_name(), logger_config.get_log_file_name(), logger_config.get_severity_level_type(), logger_config.get_rotation_size(), logger_config.is_console_enabled());
}