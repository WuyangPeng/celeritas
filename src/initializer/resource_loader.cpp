#include "resource_loader.h"
#include "common/logger.h"
#include "common/random_helper.h"
#include "database/database_pool_manager.h"
#include "detail/database_resource_loader.h"
#include "detail/logger_resource_loader.h"
#include "detail/server_resource_loader.h"
#include "detail/service_registry_loader.h"
#include "network/tcp_client.h"
#include "server/server_fwd.h"
#include "service_registry/detail/service_registry_internal_fwd.h"

#include <ranges>
#include <utility>

celeritas::resource_loader::resource_loader(app_config_shared_ptr app_config)
    : app_config_{ std::move(app_config) }, listener_{}, tcp_clients_{}, is_service_registry_{ false }, game_server_id_{}, timer_interval_{}
{
}

void celeritas::resource_loader::initialize(boost::asio::io_context& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
    initialize_logger_resource();
    initialize_server_resource(io_context, network_message_callback);
    initialize_database_resource(io_context);
    initialize_health_check_url_resource();
    initialize_service_registry_resource(io_context, network_message_callback);
    start_check_tcp_clients_timer(io_context);
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

        if (!service_registry.empty())
        {
            const auto random_index = random_helper::get_random_int(service_registry.size());

            auto iter = service_registry.begin();
            std::advance(iter, random_index);

            const auto client = service_registry_loader::loader_service_registry(io_context, iter->second, network_message_callback, game_server_id_, service_registry_type.data());

            tcp_clients_.emplace_back(client);
        }
    }
}

void celeritas::resource_loader::start_check_tcp_clients_timer(boost::asio::io_context& io_context)
{
    timer_interval_ = std::make_unique<steady_timer_type>(io_context);

    start_check_tcp_clients_timer(io_context, shared_from_this());
}

void celeritas::resource_loader::start_check_tcp_clients_timer(boost::asio::io_context& io_context, const self_shared_ptr& self)
{
    timer_interval_->expires_at(std::chrono::steady_clock::now() + check_tcp_clients_timer);
    timer_interval_->async_wait(
        [self,&io_context](const error_code_type& error_code) {
            if (!error_code)
            {
                self->check_tcp_clients(io_context, error_code);
            }
        });
}

void celeritas::resource_loader::check_tcp_clients(boost::asio::io_context& io_context, const error_code_type& error_code)
{
    if (error_code == boost::asio::error::operation_aborted)
    {
        return;
    }

    const auto self{ shared_from_this() };

    process_check_tcp_clients(io_context);

    start_check_tcp_clients_timer(io_context, self);
}

void celeritas::resource_loader::process_check_tcp_clients(boost::asio::io_context& io_context)
{
    try
    {
        process_check_tcp_clients_by_duration(io_context);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(initializer_channel, error) << "check tcp clients error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(initializer_channel, fatal) << "check tcp clients error: an unknown exception";
    }
}

void celeritas::resource_loader::process_check_tcp_clients_by_duration(boost::asio::io_context& io_context)
{
    for (auto iter = tcp_clients_.begin(); iter != tcp_clients_.end();)
    {
        auto& tcp_client = *iter;
        if (!tcp_client->is_open())
        {
            if (tcp_client->get_server_type() == service_registry_type)
            {
                iter = tcp_clients_.erase(iter);

                initialize_service_registry_resource(io_context, tcp_client->get_network_message_callback());
            }
            else
            {
                boost::asio::co_spawn(
                    io_context,
                    tcp_client->connect(),
                    boost::asio::detached
                    );

                ++iter;
            }
        }
        else
        {
            ++iter;
        }
    }
}