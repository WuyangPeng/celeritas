#include "resource_loader.h"
#include "boost/beast/http/message_fwd.hpp"
#include "common/logger.h"
#include "common/random_helper.h"
#include "database/database_pool_manager.h"
#include "detail/buffer_pool_timer.h"
#include "detail/check_tcp_clients_timer.h"
#include "detail/database_resource_loader.h"
#include "detail/logger_resource_loader.h"
#include "detail/server_resource_loader.h"
#include "detail/service_registry_loader.h"
#include "detail/service_registry_timer.h"
#include "../handler/health_check_level_type.h"
#include "network/tcp_client.h"
#include "proto/celeritas.pb.h"
#include "server/server_fwd.h"
#include "service_registry/detail/service_registry_internal_fwd.h"
#include "service_registry_server/service_registry_server.h"

#include <ranges>

celeritas::resource_loader::resource_loader(app_config_shared_ptr app_config)
    : app_config_{ std::move(app_config) },
      listener_{},
      tcp_clients_{},
      is_service_registry_{ false },
      check_tcp_clients_timer_{},
      service_registry_timer_{},
      buffer_pool_timer_{},
      start_server_time_{ std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count() }
{
}

void celeritas::resource_loader::initialize(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
    initialize_logger_resource();
    initialize_server_resource(io_context, network_message_callback);
    initialize_database_resource(io_context);
    initialize_service_registry_resource(io_context, network_message_callback);
    start_check_tcp_clients_timer(io_context);
    start_service_registry_timer(io_context);
    start_buffer_pool_timer(io_context);
    service_initialize_resource(io_context, network_message_callback);
}

void celeritas::resource_loader::release_resource()
{
    for (const auto& element : tcp_clients_)
    {
        if (element->get_server_type() == service_registry_type)
        {
            proto::celeritas request{};
            request.mutable_celeritas_request()->mutable_service()->mutable_registry()->mutable_server_close();
            element->write(header{ proto::common::empty_message_header{} }, request);
        }

        element->stop();
    }

    tcp_clients_.clear();

    for (const auto& element : listener_)
    {
        element->stop();
    }

    listener_.clear();

    if (check_tcp_clients_timer_ != nullptr)
    {
        check_tcp_clients_timer_->stop();
    }

    if (service_registry_timer_ != nullptr)
    {
        service_registry_timer_->stop();
    }
}

bool celeritas::resource_loader::write(const std::string& server_type, const header& header, const protobuf_message& request) const
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

void celeritas::resource_loader::process_check_tcp_clients_by_duration(io_context_type& io_context)
{
    for (auto index = 0; index < tcp_clients_.size(); ++index)
    {
        if (const auto& tcp_client = tcp_clients_[index];
            !tcp_client->is_open())
        {
            if (!is_service_registry_ && tcp_client->get_server_type() == service_registry_type)
            {
                modify_service_registry_resource(io_context, tcp_client->get_network_message_callback(), index);
            }
            else
            {
                boost::asio::co_spawn(io_context,
                                      tcp_client->connect(),
                                      boost::asio::detached);
            }
        }
    }
}

void celeritas::resource_loader::process_service_registry_by_duration()
{
    proto::celeritas request{};
    auto* server_register = request.mutable_celeritas_request()->mutable_service()->mutable_registry()->mutable_server_register();
    const auto server = app_config_->get_server_config();

    server_register->set_service_name(server.get_service_name());
    server_register->set_instance_id(server.get_instance_id());
    server_register->set_game_server_id(server.get_game_server_id());
    server_register->set_host(server.get_host());
    server_register->set_start_server_time(start_server_time_);

    for (const auto& element : server)
    {
        auto* port = server_register->add_port();
        port->set_protocol(static_cast<int>(element.get_server_network_type()));
        port->set_port(element.get_port());
    }

    if (write(service_registry_type.data(), header{ proto::common::empty_message_header{} }, request))
    {
        LOG_CHANNEL(initializer_channel, trace) << "service registry registry: " << server.get_instance_id();
    }
}

celeritas::resource_loader::app_config_shared_ptr celeritas::resource_loader::get_app_config() const
{
    return app_config_;
}

celeritas::resource_loader::health_check_level_awaitable_type celeritas::resource_loader::get_health_check_level() const
{
    if (const auto is_health = co_await database_pool_manager::get_instance().is_health();
        !is_health)
    {
        co_return health_check_level_type::crash;
    }

    for (const auto& element : tcp_clients_)
    {
        if (element->is_full())
        {
            co_return health_check_level_type::unhealthy;
        }
    }

    co_return health_check_level_type::health;
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

void celeritas::resource_loader::initialize_database_resource(io_context_type& io_context)
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

void celeritas::resource_loader::initialize_server_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback)
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

void celeritas::resource_loader::initialize_service_registry_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
    const auto service_registry = app_config_->get_service_registry_config();

    if (!is_service_registry_)
    {
        if (!service_registry.empty())
        {
            const auto client = get_random_client(io_context, network_message_callback, service_registry);

            tcp_clients_.emplace_back(client);
        }
    }
    else
    {
        const auto server = app_config_->get_server_config();
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

void celeritas::resource_loader::modify_service_registry_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback, int index)
{
    if (const auto service_registry = app_config_->get_service_registry_config();
        !service_registry.empty())
    {
        tcp_clients_.at(index) = get_random_client(io_context, network_message_callback, service_registry);
    }
}

void celeritas::resource_loader::start_check_tcp_clients_timer(io_context_type& io_context)
{
    check_tcp_clients_timer_ = std::make_unique<check_tcp_clients_timer>(io_context, check_tcp_clients_seconds, shared_from_this());

    check_tcp_clients_timer_->start();
}

void celeritas::resource_loader::start_service_registry_timer(io_context_type& io_context)
{
    service_registry_timer_ = std::make_unique<service_registry_timer>(io_context, service_registry_seconds, shared_from_this());

    service_registry_timer_->start(true);
}

void celeritas::resource_loader::start_buffer_pool_timer(io_context_type& io_context)
{
    buffer_pool_timer_ = std::make_unique<buffer_pool_timer>(io_context, buffer_pool_seconds);

    buffer_pool_timer_->start();
}

celeritas::resource_loader::tcp_client_shared_ptr celeritas::resource_loader::get_random_client(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback, const service_registry_config_container& service_registry) const
{
    const auto random_index = random_helper::get_random_int(service_registry.size());

    auto iter = service_registry.begin();
    std::advance(iter, random_index);

    const auto server = app_config_->get_server_config();
    return service_registry_loader::loader_service_registry(io_context, iter->second, network_message_callback, server.get_game_server_id(), service_registry_type.data());
}
