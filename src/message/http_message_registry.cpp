#include "http_base_message_handler.h"
#include "http_handle_parameter.h"
#include "http_message_registry.h"

void celeritas::http_message_registry::registerHandler(const http_base_message_handler_shared_ptr& handler)
{
    const auto typeName = handler->get_supported_type_name();

    std::lock_guard lock{ mutex_ };

    registry_[typeName] = handler;
}

bool celeritas::http_message_registry::dispatch(const http_handle_parameter& handle_parameter)
{
    if (const auto http_base_message_handler = get_http_base_message_handler(handle_parameter))
    {
        return http_base_message_handler->get()->handle(handle_parameter, shared_from_this());
    }

    return false;
}

celeritas::http_message_registry::http_base_message_handler_optional_type celeritas::http_message_registry::get_http_base_message_handler(const http_handle_parameter& handle_parameter)
{
    const auto typeName = handle_parameter.get_path();

    std::lock_guard lock{ mutex_ };

    if (const auto iter = registry_.find(typeName);
        iter != registry_.end())
    {
        return iter->second;
    }

    return std::nullopt;
}