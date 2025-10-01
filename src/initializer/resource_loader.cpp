#include "resource_loader.h"
#include "database/database_pool_manager.h"
#include "detail/database_resource_loader.h"
#include "detail/logger_resource_loader.h"

#include <ranges>

celeritas::resource_loader::resource_loader(const app_config_shared_ptr& app_config)
    : app_config_{ app_config }
{
}

void celeritas::resource_loader::initialize(boost::asio::io_context& io_context)
{
    initialize_logger_resource();
    initialize_database_resource(io_context);
    initialize_server_resource();
    initialize_health_check_url_resource();
    initialize_service_registry_resource();
    service_initialize_resource();
}

void celeritas::resource_loader::initialize_logger_resource()
{
    logger_resource_loader::loader_level_config(app_config_->get_logger_level_config());

    for (const auto logger = app_config_->get_logger_config();
         const auto& element : logger | std::views::values)
    {
        logger_resource_loader::loader_logger(element);
    }
}

void celeritas::resource_loader::initialize_database_resource(boost::asio::io_context& io_context)
{
    const auto database = app_config_->get_database_config();
    for (const auto& element : database | std::views::values)
    {
        database_resource_loader::loader_database(io_context, element);
    }

    if (!database.empty())
    {
        database_pool_manager::get_instance().start_cleanup_timer(io_context);
    }
}

void celeritas::resource_loader::initialize_server_resource()
{
}

void celeritas::resource_loader::initialize_health_check_url_resource()
{
}

void celeritas::resource_loader::initialize_service_registry_resource()
{
}