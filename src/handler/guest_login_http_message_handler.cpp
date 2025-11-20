#include "guest_login_http_message_handler.h"

std::string celeritas::guest_login_http_message_handler::get_supported_type_name() const
{
    return "guest_login";
}

bool celeritas::guest_login_http_message_handler::handle(const http_handle_parameter& handle_parameter, const http_message_registry_weak_ptr& message_registry)
{
    return true;
}

