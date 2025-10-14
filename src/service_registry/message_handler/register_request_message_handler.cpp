#include "register_request_message_handler.h"
#include "message/concrete_message_handler.tpp"

bool celeritas::register_request_message_handler::handle_concrete(const header& header, const message_shared_ptr& message, const message_registry_weak_ptr& message_registry)
{
    return true;
}
