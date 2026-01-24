#include "service_login_request_message_handler.h"
#include "initializer/initializer_constant.h"
#include "message/handler_base/concrete_message_handler.tpp"
#include "player/login/service_login.h"

celeritas::service_login_request_message_handler::service_login_request_message_handler()
{
}

std::string celeritas::service_login_request_message_handler::get_server_type() const
{
    return player_type.data();
}

bool celeritas::service_login_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    co_spawn_response<service_login>(handle_parameter, current_message, handler_channel, "service login error:");

    return true;
}
