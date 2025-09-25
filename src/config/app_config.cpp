#include "app_config.h"
#include "common/logger.h"
#include "detail/health_check_url_config_reader.h"
#include "detail/server_config_reader.h"
#include "detail/service_registry_config_reader.h"
#include "common/common_fwd.h"
#include "detail/database_config_reader.h"

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
        for (const auto& result = database_config_reader::load_config(filename);
             const auto& element : result)
        {
            database_[element.get_name()] = element;
        }
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(initializer_channel, error) << "load databases config error:" << error.what();
        throw;
    }
}