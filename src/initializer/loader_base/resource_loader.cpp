#include "resource_loader.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "common/core/random_helper.h"
#include "common/logging/logger.h"
#include "config/game/game_config.h"
#include "config/local/service_registry_config.h"
#include "config/luban/generated/schema.h"
#include "database/pool/database_pool_manager.h"
#include "detail/buffer_pool_timer.h"
#include "detail/check_tcp_clients_timer.h"
#include "detail/database_resource_loader.h"
#include "detail/logger_resource_loader.h"
#include "detail/server_resource_loader.h"
#include "detail/service_registry_loader.h"
#include "detail/service_registry_timer.h"
#include "initializer/initializer_constant.h"
#include "network/client/tcp_client.h"
#include "proto/celeritas.pb.h"
#include "log/core/log_client.h"
#include "service_registry/core/detail/service_registry_core_internal_constant.h"
#include "service_registry/data/health_check_level_type.h"
#include "service_registry/data/service_info.h"

#include <ranges>

celeritas::resource_loader::resource_loader(const std::string_view server_type, const_app_config_shared_ptr app_config)
    : app_config_{ std::move(app_config) },
      listener_{},
      tcp_clients_{},
      is_service_registry_{ false },
      check_tcp_clients_timer_{},
      service_registry_timer_{},
      buffer_pool_timer_{},
      start_server_time_{ std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count() },
      server_type_{ server_type },
      session_route_{},
      session_mapping_{},
      network_message_callback_{},
      mutex_{}
{
}

std::string_view celeritas::resource_loader::get_server_type() const
{
    return server_type_;
}

void celeritas::resource_loader::initialize(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback)
{
    network_message_callback_ = network_message_callback;

    initialize_logger_resource();
    initialize_server_resource(any_io_executor, network_message_callback);
    initialize_database_resource(any_io_executor);
    initialize_service_registry_resource(any_io_executor, network_message_callback);
    start_check_tcp_clients_timer(any_io_executor);
    start_service_registry_timer(any_io_executor);
    start_buffer_pool_timer(any_io_executor);
    initialize_game_config();
    log_client::get_instance().initialize(any_io_executor, shared_from_this());
    service_initialize_resource(any_io_executor, network_message_callback);
}

void celeritas::resource_loader::release_resource()
{
    std::lock_guard lock{ mutex_ };

    for (const auto& element : tcp_clients_ | std::views::values)
    {
        if (element->get_server_type() == service_registry_type)
        {
            proto::celeritas request{};
            request.mutable_celeritas_request()->mutable_service()->mutable_registry()->mutable_server_close();
            element->write(header{}, request);
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

    log_client::get_instance().stop();
}

bool celeritas::resource_loader::write_to_server(const std::string& server_type, const header& header, const protobuf_message& request)
{
    std::shared_lock lock{ mutex_ };

    auto to_write = false;

    for (const auto& element : tcp_clients_ | std::views::values)
    {
        if (element->get_server_type() == server_type)
        {
            element->write(header, request);
            to_write = true;
        }
    }

    return to_write;
}

bool celeritas::resource_loader::write(const header& header, const protobuf_message& request)
{
    std::shared_lock lock{ mutex_ };

    auto to_write = false;

    for (const auto& element : tcp_clients_ | std::views::values)
    {
        if (!element->get_server_type().empty())
        {
            element->write(header, request);
            to_write = true;
        }
    }

    return to_write;
}

bool celeritas::resource_loader::write_to_server(const std::string& server_type, const std::string& instance_id, const header& header, const protobuf_message& request)
{
    std::shared_lock lock{ mutex_ };

    auto to_write = false;

    for (const auto& element : tcp_clients_ | std::views::values)
    {
        if (element->get_server_type() == server_type && element->get_instance_id() == instance_id)
        {
            element->write(header, request);
            to_write = true;
        }
    }

    if (app_config_->get_server_config()->get_service_name() != gateway_type)
    {
        for (const auto& element : listener_)
        {
            if (element->get_server_network_type() == server_network_type::tcp)
            {
                if (element->write(server_type, instance_id, header, request))
                {
                    to_write = true;
                }
            }
        }
    }

    return to_write;
}

bool celeritas::resource_loader::write_to_client(const header& header, const protobuf_message& response)
{
    std::shared_lock lock{ mutex_ };

    auto to_write = false;

    if (const auto iter = session_route_.find(header.get_user_id());
        iter != session_route_.cend())
    {
        for (const auto& element : listener_)
        {
            if (element->get_server_network_type() == iter->second.get_server_network_type())
            {
                const auto session = element->get_session(iter->second.get_session_id());
                session->write(header, response);
                to_write = true;

                break;
            }
        }
    }

    return to_write;
}

bool celeritas::resource_loader::write_to_user(const std::string& server_type, const int64_t session_id, const header& header_message, const protobuf_message& message)
{
    std::shared_lock lock{ mutex_ };

    if (const auto iter = session_mapping_.find(session_id);
        iter != session_mapping_.cend())
    {
        if (const auto session = session_route_.find(iter->second); session != session_route_.cend())
        {
            return write_to_server(server_type, session->second.get_instance_id(), header{ header_message.get_rpc(), iter->second }, message);
        }
    }

    return false;
}

void celeritas::resource_loader::process_check_tcp_clients_by_duration(const any_io_executor& any_io_executor)
{
    std::vector<tcp_client_shared_ptr> no_open_clients{};
    {
        std::shared_lock lock{ mutex_ };
        for (const auto& element : tcp_clients_ | std::views::values)
        {
            if (!element->is_open())
            {
                no_open_clients.emplace_back(element);
            }
        }
    }

    for (const auto& tcp_client : no_open_clients)
    {
        if (!is_service_registry_ && tcp_client->get_server_type() == service_registry_type)
        {
            modify_service_registry_resource(any_io_executor, tcp_client->get_network_message_callback(), tcp_client->get_instance_id());
        }
        else
        {
            boost::asio::co_spawn(any_io_executor,
                                  noexcept_safe_call_and_log_awaitable([tcp_client = tcp_client] {
                                                                           return tcp_client->connect();
                                                                       },
                                                                       initializer_channel,
                                                                       "tcp client connect error:"),

                                  boost::asio::detached);
        }
    }
}

void celeritas::resource_loader::process_service_registry_by_duration()
{
    proto::celeritas request{};
    auto* server_register = request.mutable_celeritas_request()->mutable_service()->mutable_registry()->mutable_server_register();
    const auto server = app_config_->get_server_config();

    server_register->set_service_name(server->get_service_name());
    server_register->set_instance_id(server->get_instance_id());
    server_register->set_game_server_id(server->get_game_server_id());
    server_register->set_host(server->get_host());
    server_register->set_external_host(app_config_->get_external_host());
    server_register->set_start_server_time(start_server_time_);

    for (const auto& element : *server)
    {
        auto* port = server_register->add_port();
        port->set_protocol(static_cast<int>(element.get_server_network_type()));
        port->set_port(element.get_port());
    }

    if (write_to_server(service_registry_type.data(), header{}, request))
    {
        LOG_CHANNEL(initializer_channel, trace) << "service registry registry: " << server->get_instance_id();
    }

    send_service_heartbeat();
}

celeritas::resource_loader::const_app_config_shared_ptr celeritas::resource_loader::get_app_config() const
{
    return app_config_;
}

celeritas::resource_loader::health_check_level_awaitable_type celeritas::resource_loader::get_health_check_level()
{
    if (const auto is_health = co_await database_pool_manager::get_instance().is_health();
        !is_health)
    {
        co_return health_check_level_type::crash;
    }

    {
        std::shared_lock lock{ mutex_ };
        for (const auto& element : tcp_clients_ | std::views::values)
        {
            if (element->is_full())
            {
                co_return health_check_level_type::unhealthy;
            }
        }
    }

    co_return health_check_level_type::health;
}

void celeritas::resource_loader::add_session_route(const int64_t user_id, const session_route& session_route)
{
    std::lock_guard lock{ mutex_ };

    if (const auto iter = session_route_.find(user_id);
        iter == session_route_.end())
    {
        session_route_.emplace(user_id, session_route);
    }
    else
    {
        session_mapping_.erase(session_route.get_session_id());
        iter->second = session_route;
    }

    session_mapping_[session_route.get_session_id()] = user_id;
}

void celeritas::resource_loader::check_client(const any_io_executor& any_io_executor, const std::string& server_type, const service_info_container& container)
{
    std::lock_guard lock{ mutex_ };

    for (auto iter = tcp_clients_.begin(); iter != tcp_clients_.end();)
    {
        if (iter->second->get_server_type() == server_type && !container.contains(iter->first))
        {
            iter = tcp_clients_.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    for (const auto& [instance_id, service_info] : container)
    {
        if (!tcp_clients_.contains(instance_id))
        {
            const auto client = std::make_shared<tcp_client>(any_io_executor,
                                                             network_message_callback_,
                                                             service_info.get_game_server_id(),
                                                             service_info.get_instance_id(),
                                                             service_info.get_host(),
                                                             service_info.get_port(server_network_type::tcp),
                                                             server_type);

            boost::asio::co_spawn(any_io_executor,
                                  client->connect(),
                                  boost::asio::detached);

            tcp_clients_.emplace(client->get_instance_id(), client);
        }
    }
}

void celeritas::resource_loader::send_offline_message(const int64_t session_id)
{
    if (const auto iter = session_mapping_.find(session_id);
        iter != session_mapping_.end())
    {
        proto::celeritas request{};
        request.mutable_celeritas_request()->mutable_service()->mutable_player()->mutable_offline();

        if (write_to_server(player_type.data(), header{ iter->second }, request))
        {
            LOG_CHANNEL(initializer_channel, debug) << "user :" << iter->second << " is offline.";
        }
    }
}

void celeritas::resource_loader::initialize_logger_resource()
{
    logger_resource_loader::loader_level_config(*app_config_->get_logger_level_config());

    for (const auto logger = app_config_->get_logger_config();
         const auto& element : *logger | std::views::values)
    {
        logger_resource_loader::loader_logger(*element);
    }
}

void celeritas::resource_loader::initialize_database_resource(const any_io_executor& any_io_executor)
{
    const auto database = app_config_->get_database_config();
    for (const auto& element : *database | std::views::values)
    {
        database_resource_loader::loader_database(any_io_executor, *element);
    }

    if (!database->empty())
    {
        database_pool_manager::get_instance().start_cleanup_timer(any_io_executor);
    }
}

void celeritas::resource_loader::initialize_server_resource(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback)
{
    const auto server = app_config_->get_server_config();

    if (server->is_service_registry_server())
    {
        is_service_registry_ = true;
    }

    for (const auto& element : *server)
    {
        const auto listener = server_resource_loader::loader_server(any_io_executor, *server, element, network_message_callback);

        listener->start();

        std::lock_guard lock{ mutex_ };

        listener_.emplace_back(listener);
    }
}

void celeritas::resource_loader::initialize_service_registry_resource(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback)
{
    const auto service_registry = app_config_->get_service_registry_config();

    if (!is_service_registry_)
    {
        if (!service_registry->empty())
        {
            const auto client = get_random_client(any_io_executor, network_message_callback, *service_registry);

            std::lock_guard lock{ mutex_ };

            tcp_clients_.emplace(client->get_instance_id(), client);
        }
    }
    else
    {
        const auto server = app_config_->get_server_config();
        const auto instance_id = server->get_instance_id();
        const auto game_server_id = server->get_game_server_id();

        for (const auto& element : *service_registry | std::views::values)
        {
            if (element->get_name() != instance_id)
            {
                const auto client = service_registry_loader::loader_service_registry(any_io_executor, *element, network_message_callback, game_server_id, service_registry_type.data());

                std::lock_guard lock{ mutex_ };
                tcp_clients_.emplace(client->get_instance_id(), client);
            }
        }
    }
}

void celeritas::resource_loader::modify_service_registry_resource(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback, const std::string& instance_id)
{
    if (const auto service_registry = app_config_->get_service_registry_config();
        !service_registry->empty())
    {
        const auto tcp_client = get_random_client(any_io_executor, network_message_callback, *service_registry);

        std::lock_guard lock{ mutex_ };

        if (tcp_client->get_instance_id() != instance_id)
        {
            tcp_clients_.erase(instance_id);
        }

        tcp_clients_[tcp_client->get_instance_id()] = tcp_client;
    }
}

void celeritas::resource_loader::start_check_tcp_clients_timer(const any_io_executor& any_io_executor)
{
    check_tcp_clients_timer_ = std::make_unique<check_tcp_clients_timer>(any_io_executor, check_tcp_clients_seconds, shared_from_this());

    check_tcp_clients_timer_->start();
}

void celeritas::resource_loader::start_service_registry_timer(const any_io_executor& any_io_executor)
{
    service_registry_timer_ = std::make_unique<service_registry_timer>(any_io_executor, service_registry_seconds, shared_from_this());

    service_registry_timer_->start(true);
}

void celeritas::resource_loader::start_buffer_pool_timer(const any_io_executor& any_io_executor)
{
    buffer_pool_timer_ = std::make_unique<buffer_pool_timer>(any_io_executor, buffer_pool_seconds);

    buffer_pool_timer_->start();
}

celeritas::resource_loader::tcp_client_shared_ptr celeritas::resource_loader::get_random_client(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback, const service_registry_config_container& service_registry) const
{
    const auto random_index = random_helper::get_random_int(service_registry.size());

    auto iter = service_registry.begin();
    std::advance(iter, random_index);

    const auto server = app_config_->get_server_config();
    return service_registry_loader::loader_service_registry(any_io_executor, *(iter->second), network_message_callback, server->get_game_server_id(), service_registry_type.data());
}

void celeritas::resource_loader::initialize_game_config()
{
    if (const auto server_config = app_config_->get_server_config();
        !server_config->is_load_game_config())
    {
        return;
    }

    game_config::get_instance().load_tables();
}

void celeritas::resource_loader::send_service_heartbeat()
{
    proto::celeritas request{};
    request.mutable_celeritas_request()->mutable_service()->mutable_registry()->mutable_service_heartbeat();

    if (write(header{}, request))
    {
        LOG_CHANNEL(initializer_channel, trace) << "service heartbeat. ";
    }
}