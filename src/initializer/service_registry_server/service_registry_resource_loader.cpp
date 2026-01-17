#include "service_registry_resource_loader.h"
#include "config/local/health_check_url_config.h"
#include "network/client/http_client.h"
#include "network/client/tcp_client.h"
#include "service_registry/core/service_registry.h"
#include "service_registry/data/service_info.h"

#include <boost/polymorphic_pointer_cast.hpp>

celeritas::service_registry_resource_loader::service_registry_resource_loader(const std::string_view server_type, const_app_config_shared_ptr app_config)
    : base_type{ server_type, std::move(app_config) }, health_check_timer_{}
{
}

void celeritas::service_registry_resource_loader::send_health_check(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback)
{
    const auto app_config = get_app_config();
    const auto health_check_url_config = app_config->get_health_check_url_config();

    for (const auto& [instance_id, service_info] : service_registry::get_services())
    {
        auto client = std::make_shared<http_client>(any_io_executor, network_message_callback, "", service_info.get_host(), service_info.get_port(server_network_type::http), service_info.get_service_name(), health_check_url_config->get_url());

        boost::asio::co_spawn(any_io_executor,
                              send_health_check(std::move(client)),
                              boost::asio::detached);
    }
}

void celeritas::service_registry_resource_loader::service_initialize_resource(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback)
{
    start_health_check_timer(any_io_executor, network_message_callback);
}

void celeritas::service_registry_resource_loader::start_health_check_timer(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback)
{
    const auto app_config = get_app_config();
    const auto health_check_url_config = app_config->get_health_check_url_config();

    health_check_timer_ = std::make_unique<health_check_timer>(any_io_executor, std::chrono::seconds(health_check_url_config->get_interval()), boost::polymorphic_pointer_downcast<class_type>(shared_from_this()), network_message_callback);

    health_check_timer_->start();
}

celeritas::service_registry_resource_loader::void_waitable_type celeritas::service_registry_resource_loader::send_health_check(const http_client_shared_ptr http_client)
{
    co_await http_client->connect();

    const auto app_config = get_app_config();
    const auto health_check_url_config = app_config->get_health_check_url_config();

    co_return co_await http_client->write_immediately("");
}