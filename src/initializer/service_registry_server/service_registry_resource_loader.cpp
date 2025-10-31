#include "service_registry_resource_loader.h"
#include "network/http_client.h"
#include "network/tcp_client.h"

#include <boost/polymorphic_pointer_cast.hpp>

celeritas::service_registry_resource_loader::service_registry_resource_loader(app_config_shared_ptr app_config)
    : base_type{ std::move(app_config) }, health_check_timer_{}
{
}

void celeritas::service_registry_resource_loader::send_health_check(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
    for (const auto tcp_client_container = get_tcp_client_container();
         const auto& element : tcp_client_container)
    {
        const auto client = std::make_shared<http_client>(io_context, network_message_callback, "", element->get_host(), element->get_port(), element->get_server_type());

        boost::asio::co_spawn(io_context,
                              send_health_check(client),
                              boost::asio::detached);
    }
}

void celeritas::service_registry_resource_loader::service_initialize_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
    start_health_check_timer(io_context, network_message_callback);
}

void celeritas::service_registry_resource_loader::start_health_check_timer(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
    const auto app_config = get_app_config();
    const auto health_check_url_config = app_config->get_health_check_url_config();

    health_check_timer_ = std::make_unique<health_check_timer>(io_context, std::chrono::seconds(health_check_url_config.get_interval()), boost::polymorphic_pointer_downcast<class_type>(shared_from_this()), network_message_callback);

    health_check_timer_->start();
}

celeritas::service_registry_resource_loader::void_waitable_type celeritas::service_registry_resource_loader::send_health_check(const http_client_shared_ptr& http_client)
{
    co_await http_client->connect();

    const auto app_config = get_app_config();
    const auto health_check_url_config = app_config->get_health_check_url_config();

    http_client->write(health_check_url_config.get_url(), "");

    co_return;
}