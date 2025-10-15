#pragma once

#include "concrete_message_handler.h"

#include <google/protobuf/descriptor.h>
#include <boost/polymorphic_pointer_cast.hpp>

template <typename Message>
std::string celeritas::concrete_message_handler<Message>::get_supported_type_name() const
{
    return Message::descriptor()->full_name().data();
}

template <typename Message>
bool celeritas::concrete_message_handler<Message>::handle(const header& header, const protobuf_message_shared_ptr& message, const message_registry_weak_ptr& message_registry)
{
    const auto concrete_message = boost::polymorphic_pointer_cast<Message>(message);

    return handle_concrete(header, concrete_message, message_registry);
}

template <typename Message>
template <typename T>
bool celeritas::concrete_message_handler<Message>::dispatch(const header& header, const T& message, const message_registry_shared_ptr& message_registry)
{
    const auto request = std::make_shared<T>();
    request->CopyFrom(message);

    if (!message_registry->dispatch(header, request))
    {
        return false;
    }
    return true;
}