#pragma once

#include "concrete_message_handler.h"
#include "protobuf_message_registry.h"
#include "common/core/celeritas_error.h"
#include "common/logger/logger.h"

#include <boost/polymorphic_cast.hpp>

template <typename Message>
std::string celeritas::concrete_message_handler<Message>::get_supported_type_name() const
{
    return Message::descriptor()->full_name().data();
}

template <typename Message>
bool celeritas::concrete_message_handler<Message>::handle(const protobuf_handle_parameter_shared_ptr& handle_parameter, const protobuf_message& current_message, const message_registry_weak_ptr& message_registry)
{
    const auto& concrete_message = boost::polymorphic_downcast<const Message&>(current_message);

    return handle_concrete(handle_parameter, concrete_message, message_registry);
}

template <typename Message>
void celeritas::concrete_message_handler<Message>::add_handler_function(int payload_case, handler_function_type handler_function)
{
    handler_.emplace(payload_case, handler_function);
}

template <typename Message>
celeritas::concrete_message_handler<Message>::handler_function_type celeritas::concrete_message_handler<Message>::get_handler_function(int payload_case) const
{
    const auto iter = handler_.find(payload_case);
    if (iter == handler_.cend())
    {
        throw celeritas_error{ "No handler function found for payload_case" };
    }

    return iter->second;
}

template <typename Message>
bool celeritas::concrete_message_handler<Message>::handle_forward(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    if (const auto message_registry_shared_ptr = message_registry.lock();
        message_registry_shared_ptr != nullptr)
    {
        const auto handler = get_handler_function(current_message.payload_case());

        return handler(handle_parameter, current_message, message_registry_shared_ptr);
    }

    return false;
}

template <typename Message>
template <typename GetFunction>
bool celeritas::concrete_message_handler<Message>::handle_dispatch(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_shared_ptr& message_registry, GetFunction get_function)
{
    if (const auto& result = (current_message.*get_function)();
        !message_registry->dispatch(handle_parameter, result))
    {
        LOG_CHANNEL(message_channel, error) << "failed to dispatch message.";
        return false;
    }

    return true;
}

