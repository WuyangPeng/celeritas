#include "service_log_request_message_handler.h"
#include "initializer/initializer_constant.h"
#include "log/core/item_flow_log_write.h"
#include "message/handler_base/concrete_message_handler.tpp"

std::string celeritas::service_log_request_message_handler::get_server_type() const
{
    return log_type.data();
}

bool celeritas::service_log_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    co_spawn_response<item_flow_log_write>(handle_parameter, current_message, handler_channel, "item flow log write error:");

    return true;
}
