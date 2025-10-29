#include "discover_response_message_handler.h"
#include "message/concrete_message_handler.tpp"

bool celeritas::discover_response_message_handler::handle_concrete(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    return true;
}