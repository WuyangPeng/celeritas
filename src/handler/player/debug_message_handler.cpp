#include "debug_message_handler.h"

bool celeritas::debug_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    return true;
}