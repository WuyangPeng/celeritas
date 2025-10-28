#include "http_base_message_handler.h"
#include "http_message_registry.h"

void celeritas::http_message_registry::registerHandler(const http_base_message_handler_shared_ptr& handler)
{
    const auto typeName = handler->get_supported_type_name();

    std::lock_guard lock{ mutex_ };

    registry_[typeName] = handler;
}

bool celeritas::http_message_registry::dispatch(const http_handle_parameter& handle_parameter)
{
    const auto typeName = handle_parameter.get_path();

    std::unique_lock lock{ mutex_ };

    if (const auto iter = registry_.find(typeName.data());
        iter != registry_.end())
    {
        lock.unlock();

        return iter->second->handle(handle_parameter, shared_from_this());
    }

    return false;
}