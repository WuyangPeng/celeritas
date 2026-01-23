#include "auth_resource_loader.h"
#include "auth/config/app_email_providers.h"
#include "auth/config/app_sdk_providers.h"
#include "auth/config/app_secret.h"
#include "auth/config/app_sms_providers.h"
#include "auth/config/server_cell_repository.h"
#include "common/logging/logger.h"
#include "config/local/health_check_url_config.h"
#include "detail/auth_server_fwd.h"
#include "initializer/initializer_constant.h"
#include "network/client/http_client.h"
#include "service_registry/core/service_registry.h"
#include "service_registry/data/service_info.h"

#include <boost/polymorphic_pointer_cast.hpp>

celeritas::auth_resource_loader::auth_resource_loader(const std::string_view server_type,
                                                      const_app_config_shared_ptr app_config)
    : base_type{ server_type, std::move(app_config) }, gateway_check_timer_{}, health_check_timer_{}
{
}

void celeritas::auth_resource_loader::send_gateway_check()
{
    proto::celeritas request{};
    auto* server_discover = request.mutable_celeritas_request()->mutable_service()->mutable_registry()->mutable_server_discover();
    server_discover->set_service_name(gateway_type);

    if (!write_to_server(service_registry_type.data(), header{ proto::common::empty_message_header{} }, request))
    {
        LOG_CHANNEL(initializer_channel, warning) << "write server discover error.";
    }
}

void celeritas::auth_resource_loader::send_health_check(const any_io_executor& any_io_executor,
                                                        const network_message_callback_weak_ptr&
                                                        network_message_callback)
{
    const auto app_config = get_app_config();
    const auto health_check_url_config = app_config->get_health_check_url_config();

    for (const auto& [instance_id, service_info] : service_registry::get_services())
    {
        auto client = std::make_shared<http_client>(any_io_executor,
                                                    network_message_callback,
                                                    "",
                                                    service_info.get_host(),
                                                    service_info.get_port(server_network_type::http),
                                                    service_info.get_service_name(),
                                                    health_check_url_config->get_url());

        boost::asio::co_spawn(any_io_executor,
                              send_health_check(std::move(client)),
                              boost::asio::detached);
    }
}

void celeritas::auth_resource_loader::service_initialize_resource(const any_io_executor& any_io_executor,
                                                                  const network_message_callback_weak_ptr&
                                                                  network_message_callback)
{
    load_from_db(any_io_executor);

    start_check_timer(any_io_executor, network_message_callback);
}

void celeritas::auth_resource_loader::load_from_db(const any_io_executor& any_io_executor)
{
    app_secret::get_instance().load_from_db(any_io_executor);
    app_sms_providers::get_instance().load_from_db(any_io_executor);
    app_email_providers::get_instance().load_from_db(any_io_executor);
    app_sdk_providers::get_instance().load_from_db(any_io_executor);
    server_cell_repository::get_instance().load_from_db(any_io_executor);
}

void celeritas::auth_resource_loader::start_check_timer(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback)
{
    start_gateway_check_timer(any_io_executor, network_message_callback);
    start_health_check_timer(any_io_executor, network_message_callback);
}

void celeritas::auth_resource_loader::start_gateway_check_timer(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback)
{
    gateway_check_timer_ = std::make_unique<gateway_check_timer>(any_io_executor,
                                                                 gateway_check_time,
                                                                 boost::polymorphic_pointer_downcast<class_type>(shared_from_this()),
                                                                 network_message_callback);

    gateway_check_timer_->start();
}

void celeritas::auth_resource_loader::start_health_check_timer(const any_io_executor& any_io_executor, const network_message_callback_weak_ptr& network_message_callback)
{
    const auto app_config = get_app_config();
    const auto health_check_url_config = app_config->get_health_check_url_config();

    health_check_timer_ = std::make_unique<auth_health_check_timer>(any_io_executor,
                                                                    std::chrono::seconds(health_check_url_config->get_interval()),
                                                                    boost::polymorphic_pointer_downcast<class_type>(shared_from_this()),
                                                                    network_message_callback);

    health_check_timer_->start();
}

celeritas::auth_resource_loader::void_waitable_type celeritas::auth_resource_loader::send_health_check(http_client_shared_ptr http_client)
{
    co_await http_client->connect();

    const auto app_config = get_app_config();
    const auto health_check_url_config = app_config->get_health_check_url_config();

    co_return co_await http_client->write_immediately("");
}