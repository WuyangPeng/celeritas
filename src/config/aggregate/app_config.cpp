#include "app_config.h"
#include "common/core/celeritas_error.h"
#include "common/core/time_helper.h"
#include "common/logging/logger.h"
#include "config/local/database_config.h"
#include "config/local/global_config.h"
#include "config/local/health_check_url_config.h"
#include "config/local/logger_config.h"
#include "config/local/logger_level_config.h"
#include "config/local/service_registry_config.h"
#include "detail/database_config_reader.h"
#include "detail/global_config_reader.h"
#include "detail/health_check_url_config_reader.h"
#include "detail/logger_config_reader.h"
#include "detail/server_config_reader.h"
#include "detail/service_registry_config_reader.h"

celeritas::app_config::app_config()
    : service_registry_{ std::make_shared<service_registry_container>() },
      server_{ std::make_shared<server_config>() },
      health_check_url_{ std::make_shared<health_check_url_config>() },
      database_{ std::make_shared<database_container>() },
      logger_level_config_{ std::make_shared<logger_level_config>() },
      logger_{ std::make_shared<logger_container>() },
      global_{ std::make_shared<global_config>() }
{
}

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
        global_ = global_config_reader::load_config(filename);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(config_channel, error) << "load global config error:" << error.what();
        throw;
    }
}

celeritas::app_config::const_service_registry_container_shared_ptr celeritas::app_config::get_service_registry_config() const
{
    return service_registry_;
}

celeritas::app_config::const_server_shared_ptr celeritas::app_config::get_server_config() const
{
    return server_;
}

celeritas::app_config::const_health_check_url_shared_ptr celeritas::app_config::get_health_check_url_config() const
{
    return health_check_url_;
}

celeritas::app_config::const_database_container_shared_ptr celeritas::app_config::get_database_config() const
{
    return database_;
}

celeritas::app_config::const_database_shared_ptr celeritas::app_config::get_database_config(const std::string& db_name) const
{
    if (const auto iter = database_->find(db_name);
        iter != database_->cend())
    {
        return iter->second;
    }

    throw celeritas_error{ "db is not exist,db name:{}", db_name };
}

celeritas::app_config::const_logger_level_shared_ptr celeritas::app_config::get_logger_level_config() const
{
    return logger_level_config_;
}

celeritas::app_config::const_logger_container_shared_ptr celeritas::app_config::get_logger_config() const
{
    return logger_;
}

int64_t celeritas::app_config::get_expire_milliseconds(const std::string& db_name) const
{
    const auto database_config = get_database_config(db_name);

    return time_helper::get_current_milliseconds() + database_config->get_expire_seconds() * milliseconds;
}

std::string celeritas::app_config::get_external_host() const
{
    return global_->get_external_host();
}

void celeritas::app_config::do_load_service_registry_config(const std::string& filename)
{
    const service_registry_config_reader service_registry_config_reader{ filename };

    for (const auto& container = service_registry_config_reader.get_service_registry_config_container();
         const auto& element : *container)
    {
        if (!service_registry_->emplace(element->get_name(), element).second)
        {
            LOG_CHANNEL(config_channel, warning) << "load service registry config repeat,name = " << element->get_name();
        }
    }
}

void celeritas::app_config::do_load_databases_config(const std::string& filename)
{
    for (const auto& container = database_config_reader::load_config(filename);
         const auto& element : *container)
    {
        (*database_)[element->get_name()] = element;
    }
}

void celeritas::app_config::do_load_loggers_config(const std::string& filename)
{
    const logger_config_reader logger_config_reader{ filename };
    for (const auto& container = logger_config_reader.get_logger_config_container();
         const auto& element : *container)
    {
        (*logger_)[element->get_name()] = element;
    }

    logger_level_config_ = logger_config_reader.get_logger_level_config();
}
