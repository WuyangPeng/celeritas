#include "base_message_handler.h"
#include "message_registry.h"

void celeritas::message_registry::registerHandler(const base_message_handler_shared_ptr& handler)
{
    std::unique_lock lock{ mutex_ };

    const auto typeName = handler->get_supported_type_name();

    registry_[typeName] = handler;
}

bool celeritas::message_registry::dispatch(const header& header, const google::protobuf::Message& current_message, const protobuf_message_shared_ptr& request_message, const session_shared_ptr& session)
{
    std::unique_lock lock{ mutex_ };

    const auto typeName = current_message.GetTypeName();

    if (const auto iter = registry_.find(typeName.data());
        iter != registry_.end())
    {
        lock.unlock();

        return iter->second->handle(header, current_message, request_message, shared_from_this(), session);
    }

    return false;
}