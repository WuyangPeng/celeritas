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
    : app_config_{ std::move(app_config) }, listener_{}, tcp_clients_{}, is_service_registry_{ false }, check_tcp_clients_timer_interval_{}, service_registry_timer_interval_{}
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
    start_service_registry_timer(io_context);
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

bool celeritas::resource_loader::write(const std::string& server_type, const header& header, const google::protobuf::Message& request) const
{
    auto to_write = false;

    for (const auto& element : tcp_clients_)
    {
        if (element->get_server_type() == server_type)
        {
            element->write(header, request);
            to_write = true;
        }
    }

    return to_write;
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

    for (const auto& element : server)
    {
        const auto listener = server_resource_loader::loader_server(io_context, server, element, network_message_callback);

        listener->start();

        listener_.emplace_back(listener);
    }
}

void celeritas::resource_loader::initialize_health_check_url_resource()
{
    const auto health_check_url = app_config_->get_health_check_url_config();
}

void celeritas::resource_loader::initialize_service_registry_resource(boost::asio::io_context& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
    const auto service_registry = app_config_->get_service_registry_config();
    const auto server = app_config_->get_server_config();

    if (!is_service_registry_)
    {
        if (!service_registry.empty())
        {
            const auto random_index = random_helper::get_random_int(service_registry.size());

            auto iter = service_registry.begin();
            std::advance(iter, random_index);

            const auto client = service_registry_loader::loader_service_registry(io_context, iter->second, network_message_callback, server.get_game_server_id(), service_registry_type.data());

            tcp_clients_.emplace_back(client);
        }
    }
    else
    {
        const auto instance_id = server.get_instance_id();
        const auto game_server_id = server.get_game_server_id();

        for (const auto& element : service_registry | std::views::values)
        {
            if (element.get_name() != instance_id)
            {
                const auto client = service_registry_loader::loader_service_registry(io_context, element, network_message_callback, game_server_id, service_registry_type.data());

                tcp_clients_.emplace_back(client);
            }
        }
    }
}

void celeritas::resource_loader::modify_service_registry_resource(boost::asio::io_context& io_context, const network_message_callback_weak_ptr& network_message_callback, int index)
{
    const auto service_registry = app_config_->get_service_registry_config();

    if (!service_registry.empty())
    {
        const auto random_index = random_helper::get_random_int(service_registry.size());

        auto iter = service_registry.begin();
        std::advance(iter, random_index);

        const auto server = app_config_->get_server_config();
        const auto client = service_registry_loader::loader_service_registry(io_context, iter->second, network_message_callback, server.get_game_server_id(), service_registry_type.data());

        tcp_clients_.at(index) = client;
    }
}

void celeritas::resource_loader::start_check_tcp_clients_timer(boost::asio::io_context& io_context)
{
    check_tcp_clients_timer_interval_ = std::make_unique<steady_timer_type>(io_context);

    start_check_tcp_clients_timer(io_context, shared_from_this());
}

void celeritas::resource_loader::start_check_tcp_clients_timer(boost::asio::io_context& io_context, const self_shared_ptr& self)
{
    check_tcp_clients_timer_interval_->expires_at(std::chrono::steady_clock::now() + check_tcp_clients_timer);
    check_tcp_clients_timer_interval_->async_wait(
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
    for (auto index = 0; index < tcp_clients_.size(); ++index)
    {
        auto& tcp_client = tcp_clients_[index];
        if (!tcp_client->is_open())
        {
            if (!is_service_registry_ && tcp_client->get_server_type() == service_registry_type)
            {
                modify_service_registry_resource(io_context, tcp_client->get_network_message_callback(), index);
            }
            else
            {
                boost::asio::co_spawn(
                    io_context,
                    tcp_client->connect(),
                    boost::asio::detached
                    );
            }
        }
    }
}

void celeritas::resource_loader::start_service_registry_timer(boost::asio::io_context& io_context)
{
    service_registry_timer_interval_ = std::make_unique<steady_timer_type>(io_context);

    start_service_registry_timer(io_context, shared_from_this());
}

void celeritas::resource_loader::start_service_registry_timer(boost::asio::io_context& io_context, const self_shared_ptr& self)
{
    service_registry_timer_interval_->expires_at(std::chrono::steady_clock::now() + service_registry_timer);
    service_registry_timer_interval_->async_wait(
        [self,&io_context](const error_code_type& error_code) {
            if (!error_code)
            {
                self->service_registry(io_context, error_code);
            }
        });
}

void celeritas::resource_loader::service_registry(boost::asio::io_context& io_context, const error_code_type& error_code)
{
    if (error_code == boost::asio::error::operation_aborted)
    {
        return;
    }

    const auto self{ shared_from_this() };

    process_service_registry();

    start_service_registry_timer(io_context, self);
}

void celeritas::resource_loader::process_service_registry()
{
    try
    {
        process_service_registry_by_duration();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(initializer_channel, error) << "service registry error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(initializer_channel, fatal) << "service registry error: an unknown exception";
    }
}

void celeritas::resource_loader::process_service_registry_by_duration()
{
    proto::request request{};
    auto* server_register = request.mutable_service()->mutable_registry()->mutable_server_register();
    const auto server = app_config_->get_server_config();

    server_register->set_service_name(server.get_service_name());
    server_register->set_instance_id(server.get_instance_id());
    server_register->set_game_server_id(server.get_game_server_id());
    server_register->set_host(server.get_host());
    for (const auto& element : server)
    {
        switch (element.get_server_network_type())
        {
            case server_network_type::tcp:
            {
                server_register->set_tcp_port(element.get_port());
                break;
            }

            case server_network_type::http:
            {
                server_register->set_http_port(element.get_port());
                break;
            }
            case server_network_type::websocket:
            {
                server_register->set_websock_port(element.get_port());
                break;
            }
            case server_network_type::tcp_ssl:
            {
                server_register->set_tcp_ssl_port(element.get_port());
                break;
            }
            case server_network_type::https:
            {
                server_register->set_https_port(element.get_port());
                break;
            }
            case server_network_type::websocket_secure:
            {
                server_register->set_websocket_secure_port(element.get_port());
                break;
            }
            default:
            {
                break;
            }
        }
    }

    if (write(service_registry_type.data(), header{ proto::common::empty_message_header{} }, request))
    {
        LOG_CHANNEL(initializer_channel, trace) << "service registry registry: " << server.get_instance_id();
    }
}