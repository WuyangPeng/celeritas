#include "gateway_login_request_message_handler.h"
#include "gateway/core/gateway_login.h"
#include "initializer/initializer_constant.h"
#include "message/handler_base/concrete_message_handler.tpp"

std::string celeritas::gateway_login_request_message_handler::get_server_type() const
{
    return gateway_type.data();
}

bool celeritas::gateway_login_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    co_spawn_response<gateway_login>(handle_parameter, handler_channel, "gateway login error:");

    return true;
}

