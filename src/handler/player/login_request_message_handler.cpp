#include "login_request_message_handler.h"
#include "message/concrete_message_handler.tpp"

celeritas::login_request_message_handler::login_request_message_handler()
{
}

bool celeritas::login_request_message_handler::handle_concrete(const protobuf_handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    return true;
}