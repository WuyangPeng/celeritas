#include "http_message_registry.h"
#include "message/handler_base/http_base_message_handler.h"
#include "message/parameters/http_handle_parameter.h"

celeritas::http_message_registry::http_message_registry(const std::string_view server_type)
    : registry_{}, mutex_{}, server_type_{ server_type }
{
}

void celeritas::http_message_registry::register_handler(const http_base_message_handler_shared_ptr& handler)
{
    if (const auto server_type = handler->get_server_type();
        !server_type.empty() && server_type.find(server_type_) == std::string::npos)
    {
        return;
    }

    const auto type_name = handler->get_supported_type_name();

    std::lock_guard lock{ mutex_ };

    registry_[type_name] = handler;
}

bool celeritas::http_message_registry::dispatch(const http_handle_parameter_shared_ptr& handle_parameter)
{
    if (const auto http_base_message_handler = get_http_base_message_handler(handle_parameter))
    {
        return (*http_base_message_handler)->handle(handle_parameter, shared_from_this());
    }

    return false;
}

celeritas::http_message_registry::http_base_message_handler_optional_type celeritas::http_message_registry::get_http_base_message_handler(const http_handle_parameter_shared_ptr& handle_parameter)
{
    const auto type_name = handle_parameter->get_path();

    std::shared_lock lock{ mutex_ };

    if (const auto iter = registry_.find(type_name);
        iter != registry_.cend())
    {
        return iter->second;
    }

    return std::nullopt;
}