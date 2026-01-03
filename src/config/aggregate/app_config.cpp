#include "app_config.h"
#include "common/core/celeritas_error.h"
#include "common/core/time_helper.h"
#include "common/logging/logger.h"
#include "detail/database_config_reader.h"
#include "detail/global_config_reader.h"
#include "detail/health_check_url_config_reader.h"
#include "detail/logger_config_reader.h"
#include "detail/server_config_reader.h"
#include "detail/service_registry_config_reader.h"

void celeritas::app_config::load_service_registry_config(const std::string& filename)
{
    try
    {
        do_load_service_registry_config(filename);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(config_channel, error) << "load service registry config error:" << error.what();
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
        LOG_CHANNEL(config_channel, error) << "load server config error:" << error.what();
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
        LOG_CHANNEL(config_channel, error) << "load health check url config error:" << error.what();
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
        LOG_CHANNEL(config_channel, error) << "load databases config error:" << error.what();
        throw;
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
        LOG_CHANNEL(config_channel, error) << "load databases config error:" << error.what();
        throw;
    }
}

void celeritas::app_config::load_global_config(const std::string& filename)
{
    try
    {
        do_load_global_config(filename);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(config_channel, error) << "load global config error:" << error.what();
        throw;
    }
}

celeritas::logger_level_config celeritas::app_config::get_logger_level_config() const
{
    return logger_level_config_;
}

celeritas::app_config::logger_config_container celeritas::app_config::get_logger_config() const
{
    return logger_;
}

celeritas::app_config::database_config_container celeritas::app_config::get_database_config() const
{
    return database_;
}

celeritas::database_config celeritas::app_config::get_database_config(const std::string& db_name) const
{
    if (const auto iter = database_.find(db_name);
        iter != database_.cend())
    {
        return iter->second;
    }

    throw celeritas_error{ "db is not exist,db name:{}", db_name };
}

celeritas::server_config celeritas::app_config::get_server_config() const
{
    return server_;
}

celeritas::health_check_url_config celeritas::app_config::get_health_check_url_config() const
{
    return health_check_url_;
}

celeritas::app_config::service_registry_config_container celeritas::app_config::get_service_registry_config() const
{
    return service_registry_;
}

int64_t celeritas::app_config::get_expire_milliseconds(const std::string& db_name) const
{
    const auto database_config = get_database_config(db_name);

    return time_helper::get_current_milliseconds() + database_config.get_expire_seconds() * milliseconds;
}

std::string celeritas::app_config::get_external_host() const
{
    return global_.get_external_host();
}

void celeritas::app_config::do_load_databases_config(const std::string& filename)
{
    for (const auto& result = database_config_reader::load_config(filename);
         const auto& element : result)
    {
        database_[element.get_name()] = element;
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

void celeritas::app_config::do_load_service_registry_config(const std::string& filename)
{
    const service_registry_config_reader service_registry_config_reader{ filename };
    for (const auto& result = service_registry_config_reader.get_service_registry_config_container();
         const auto& element : result)
    {
        service_registry_[element.get_name()] = element;
    }
}

void celeritas::app_config::do_load_global_config(const std::string& filename)
{
    global_ = global_config_reader::load_config(filename);
}