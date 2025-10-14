#include "message_registry.h"

void celeritas::message_registry::registerHandler(const base_message_handler_shared_ptr& handler)
{
    std::unique_lock lock{ mutex_ };

    const auto typeName = handler->get_supported_type_name();

    registry_[typeName] = handler;
}

bool celeritas::message_registry::dispatch(const header& header, const protobuf_message_shared_ptr& message)
{
    std::unique_lock lock{ mutex_ };

    const auto typeName = message->GetTypeName();

    if (const auto iter = registry_.find(typeName.data());
        iter != registry_.end())
    {
        return iter->second->handle(header, message);
    }

    return false;
}