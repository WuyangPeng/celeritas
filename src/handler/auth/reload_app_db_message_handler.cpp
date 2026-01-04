#include "reload_app_db_message_handler.h"
#include "auth/data/app_secret.h"
#include "message/parameters/protobuf_handle_parameter.h"
#include "proto/celeritas.pb.h"

bool celeritas::reload_app_db_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter,
                                                               const message_type& current_message,
                                                               const message_registry_weak_ptr& message_registry)
{
    app_secret::get_instance().reload_from_db(handle_parameter->get_io_context(), current_message.app_id());

    proto::celeritas response{};
    response.mutable_celeritas_response()->mutable_service()->mutable_auth()->mutable_reload_app_db();
    handle_parameter->write(response);

    return true;
}