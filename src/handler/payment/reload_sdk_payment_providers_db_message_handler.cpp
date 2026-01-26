#include "reload_sdk_payment_providers_db_message_handler.h"
#include "initializer/application_loader.h"
#include "manual/notify_http_message_handler.h"
#include "manual/refund_http_message_handler.h"
#include "message/parameters/protobuf_handle_parameter.h"
#include "payment/config/app_sdk_payment_providers.h"
#include "proto/celeritas.pb.h"

#include <boost/polymorphic_pointer_cast.hpp>

bool celeritas::reload_sdk_payment_providers_db_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    app_sdk_payment_providers::get_instance().reload_from_db(handle_parameter->get_any_io_executor(), current_message.sdk_id());

    const auto sdk_payment_provider = app_sdk_payment_providers::get_instance().get_sdk_payment_providers(current_message.sdk_id());

    const auto application_loader_shared_ptr = boost::polymorphic_pointer_downcast<application_loader>(handle_parameter->get_application_loader());

    const sdk_payment_providers_key sdk_payment_providers_key{ sdk_payment_provider.get_app_id(), static_cast<payment_platform_type>(sdk_payment_provider.get_platform()) };
    application_loader_shared_ptr->register_handler(std::make_shared<notify_http_message_handler>(sdk_payment_providers_key, sdk_payment_provider.get_http_suffix()));
    application_loader_shared_ptr->register_handler(std::make_shared<refund_http_message_handler>(sdk_payment_providers_key, sdk_payment_provider.get_http_suffix()));

    proto::celeritas response{};
    response.mutable_celeritas_response()->mutable_service()->mutable_payment()->mutable_reload_sdk_payment_providers_db();
    handle_parameter->write_to_response(response);

    return true;
}