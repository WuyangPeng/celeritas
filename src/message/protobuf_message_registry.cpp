#include "protobuf_base_message_handler.h"
#include "protobuf_message_registry.h"

#include <mutex>

void celeritas::protobuf_message_registry::register_handler(const base_message_handler_shared_ptr& handler)
{
    const auto typeName = handler->get_supported_type_name();

    std::lock_guard lock{ mutex_ };

    registry_[typeName] = handler;
}

bool celeritas::protobuf_message_registry::dispatch(const protobuf_handle_parameter& handle_parameter, const protobuf_message& current_message)
{
    if (const auto base_message_handler = get_base_message_handler(current_message))
    {
        return base_message_handler->get()->handle(handle_parameter, current_message, shared_from_this());
    }

    return false;
}

celeritas::protobuf_message_registry::base_message_handler_optional_type celeritas::protobuf_message_registry::get_base_message_handler(const protobuf_message& current_message)
{
    const auto typeName = current_message.GetTypeName();

    std::shared_lock lock{ mutex_ };

    if (const auto iter = registry_.find(typeName.data());
        iter != registry_.cend())
    {
        return iter->second;
    }

    return std::nullopt;
}