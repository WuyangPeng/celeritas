#include "reload_app_db_message_handler.h"
#include "auth/app_secret.h"
#include "message/protobuf_handle_parameter.h"

bool celeritas::reload_app_db_message_handler::handle_concrete(const protobuf_handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    app_secret::get_instance().reload_from_db(handle_parameter.get_io_context(), current_message.app_id());

    return true;
}