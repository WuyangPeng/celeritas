#include "reload_sms_providers_db_message_handler.h"
#include "auth/app_sms_providers.h"
#include "message/protobuf_handle_parameter.h"
#include "proto/celeritas.pb.h"

bool celeritas::reload_sms_providers_db_message_handler::handle_concrete(const protobuf_handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    app_sms_providers::get_instance().reload_from_db(handle_parameter.get_io_context(), current_message.provider_id());

    proto::celeritas response{};
    response.mutable_celeritas_response()->mutable_service()->mutable_auth()->mutable_reload_sms_providers_db();
    handle_parameter.write(response);

    return true;
}