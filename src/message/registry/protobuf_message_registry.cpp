#include "protobuf_message_registry.h"
#include "message/handler_base/protobuf_base_message_handler.h"

#include <mutex>

celeritas::protobuf_message_registry::protobuf_message_registry(const std::string_view server_type)
    : registry_{}, mutex_{}, server_type_{ server_type }
{
}

void celeritas::protobuf_message_registry::register_handler(const base_message_handler_shared_ptr& handler)
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

bool celeritas::protobuf_message_registry::dispatch(const protobuf_handle_parameter_shared_ptr& handle_parameter, const protobuf_message& current_message)
{
    if (const auto base_message_handler = get_base_message_handler(current_message))
    {
        return (*base_message_handler)->handle(handle_parameter, current_message, shared_from_this());
    }

    return false;
}

celeritas::protobuf_message_registry::base_message_handler_optional_type celeritas::protobuf_message_registry::get_base_message_handler(const protobuf_message& current_message)
{
    const auto type_name = current_message.GetTypeName();

    std::shared_lock lock{ mutex_ };

    if (const auto iter = registry_.find(type_name.data());
        iter != registry_.cend())
    {
        return iter->second;
    }

    return std::nullopt;
}