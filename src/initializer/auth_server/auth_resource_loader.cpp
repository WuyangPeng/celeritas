#include "auth_resource_loader.h"
#include "auth/app_email_providers.h"
#include "auth/app_sdk_providers.h"
#include "auth/app_secret.h"
#include "auth/app_sms_providers.h"
#include "auth/server_cell_repository.h"
#include "common/logger.h"
#include "detail/auth_server_fwd.h"
#include "network/http_client.h"
#include "service_registry/service_info.h"
#include "service_registry/service_registry.h"

#include <boost/polymorphic_pointer_cast.hpp>

celeritas::auth_resource_loader::auth_resource_loader(const std::string_view server_type, app_config_shared_ptr app_config)
    : base_type{ server_type, std::move(app_config) }, gateway_check_timer_{}
{
}

void celeritas::auth_resource_loader::send_gateway_check()
{
    proto::celeritas request{};
    auto* server_discover = request.mutable_celeritas_request()->mutable_service()->mutable_registry()->mutable_server_discover();
    server_discover->set_service_name(service_registry_type);

    if (!write(service_registry_type.data(), header{ proto::common::empty_message_header{} }, request))
    {
        LOG_CHANNEL(initializer_channel, warning) << "write server discover error.";
    }
}

void celeritas::auth_resource_loader::service_initialize_resource(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
    load_from_db(io_context);

    start_gateway_check_timer(io_context, network_message_callback);
}

void celeritas::auth_resource_loader::load_from_db(io_context_type& io_context)
{
    app_secret::get_instance().load_from_db(io_context);
    app_sms_providers::get_instance().load_from_db(io_context);
    app_email_providers::get_instance().load_from_db(io_context);
    app_sdk_providers::get_instance().load_from_db(io_context);
    server_cell_repository::get_instance().load_from_db(io_context);
}

void celeritas::auth_resource_loader::start_gateway_check_timer(io_context_type& io_context, const network_message_callback_weak_ptr& network_message_callback)
{
    const auto app_config = get_app_config();
    const auto health_check_url_config = app_config->get_health_check_url_config();

    gateway_check_timer_ = std::make_unique<gateway_check_timer>(io_context, gateway_check_time, boost::polymorphic_pointer_downcast<class_type>(shared_from_this()), network_message_callback);

    gateway_check_timer_->start();
}

