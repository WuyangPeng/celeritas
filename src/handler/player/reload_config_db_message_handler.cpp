#include "reload_config_db_message_handler.h"
#include "database/config/config_manager.h"
#include "message/protobuf_handle_parameter.h"
#include "proto/celeritas.pb.h"

bool celeritas::reload_config_db_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter,
                                                                  const message_type& current_message,
                                                                  const message_registry_weak_ptr& message_registry)
{
    config_manager::get_instance().reload_from_db(handle_parameter->get_io_context(), current_message.db_name(), current_message.id());

    proto::celeritas response{};
    response.mutable_celeritas_response()->mutable_service()->mutable_player()->mutable_reload_config_db();
    handle_parameter->write(response);

    return true;
}