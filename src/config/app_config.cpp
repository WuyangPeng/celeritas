#include "app_config.h"
#include "common/logger.h"
#include "detail/health_check_url_config_reader.h"
#include "detail/server_config_reader.h"
#include "detail/service_registry_config_reader.h"
#include "common/common_fwd.h"
#include "detail/database_config_reader.h"
#include "detail/logger_config_reader.h"

void celeritas::app_config::load_service_registry_config(const std::string& filename)
{
    try
    {
        service_registry_ = service_registry_config_reader::load_config(filename);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(initializer_channel, error) << "load service registry config error:" << error.what();
        throw;
    }
}

void celeritas::app_config::load_server_config(const std::string& filename)
{
    try
    {
        server_ = server_config_reader::load_config(filename);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(initializer_channel, error) << "load server config error:" << error.what();
        throw;
    }
}

void celeritas::app_config::load_health_check_url_config(const std::string& filename)
{
    try
    {
        health_check_url_ = health_check_url_config_reader::load_config(filename);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(initializer_channel, error) << "load health check url config error:" << error.what();
        throw;
    }
}


void celeritas::app_config::load_databases_config(const std::string& filename)
{
    try
    {
        do_load_databases_config(filename);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(initializer_channel, error) << "load databases config error:" << error.what();
        throw;
    }
}

void celeritas::app_config::do_load_databases_config(const std::string& filename)
{
    for (const auto& result = database_config_reader::load_config(filename);
         const auto& element : result)
    {
        database_[element.get_name()] = element;
    }
}


void celeritas::app_config::load_loggers_config(const std::string& filename)
{
    try
    {
        do_load_loggers_config(filename);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(initializer_channel, error) << "load databases config error:" << error.what();
        throw;
    }
}

void celeritas::app_config::do_load_loggers_config(const std::string& filename)
{
    const logger_config_reader logger_config_reader{ filename };
    for (const auto& result = logger_config_reader.get_logger_config_container();
         const auto& element : result)
    {
        logger_[element.get_name()] = element;
    }

    const auto logger_level_config = logger_config_reader.get_logger_level_config();

    if (logger_level_config.is_set_default_level())
    {
        logger_level_config_.set_default_level(logger_level_config.get_default_level());
    }

    if (logger_level_config.is_set_console_level())
    {
        logger_level_config_.set_console_level(logger_level_config.get_console_level());
    }
}