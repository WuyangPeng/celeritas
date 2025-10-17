#include "base_message_handler.h"
#include "message_registry.h"

void celeritas::message_registry::registerHandler(const base_message_handler_shared_ptr& handler)
{
    std::unique_lock lock{ mutex_ };

    const auto typeName = handler->get_supported_type_name();

    registry_[typeName] = handler;
}

bool celeritas::message_registry::dispatch(const handle_parameter& handle_parameter, const google::protobuf::Message& current_message)
{
    std::unique_lock lock{ mutex_ };

    const auto typeName = current_message.GetTypeName();

    if (const auto iter = registry_.find(typeName.data());
        iter != registry_.end())
    {
        lock.unlock();

        return iter->second->handle(handle_parameter, current_message, shared_from_this());
    }

    return false;
}