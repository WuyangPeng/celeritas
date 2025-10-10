#include "resource_loader.h"
#include "database/database_pool_manager.h"
#include "detail/database_resource_loader.h"
#include "detail/logger_resource_loader.h"
#include "detail/server_resource_loader.h"
#include "detail/service_registry_loader.h"

#include <ranges>

celeritas::resource_loader::resource_loader(const app_config_shared_ptr& app_config)
    : app_config_{ app_config }, listener_{}, tcp_clients_{}, is_service_registry_{ false }, game_server_id_{}
{
}

void celeritas::resource_loader::initialize(boost::asio::io_context& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
    initialize_logger_resource();
    initialize_server_resource(io_context, network_message_callback);
    initialize_database_resource(io_context);
    initialize_health_check_url_resource();
    initialize_service_registry_resource(io_context, network_message_callback);
    service_initialize_resource();
}

void celeritas::resource_loader::release_resource()
{
    for (const auto& element : listener_)
    {
        element->stop();
    }

    listener_.clear();
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

void celeritas::resource_loader::initialize_server_resource(boost::asio::io_context& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
    const auto server = app_config_->get_server_config();

    if (server.is_service_registry_server())
    {
        is_service_registry_ = true;
    }

    game_server_id_ = server.get_game_server_id();

    for (const auto& element : server)
    {
        const auto listener = server_resource_loader::loader_server(io_context, server, element, network_message_callback);

        listener_.emplace_back(listener);
    }
}

void celeritas::resource_loader::initialize_health_check_url_resource()
{
    const auto health_check_url = app_config_->get_health_check_url_config();
}

void celeritas::resource_loader::initialize_service_registry_resource(boost::asio::io_context& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
    if (!is_service_registry_)
    {
        const auto service_registry = app_config_->get_service_registry_config();

        const auto client = service_registry_loader::loader_service_registry(io_context, service_registry, network_message_callback, game_server_id_);

        tcp_clients_.emplace_back(client);
    }
}