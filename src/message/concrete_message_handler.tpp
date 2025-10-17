#pragma once

#include "concrete_message_handler.h"

#include <boost/polymorphic_cast.hpp>

template <typename Message>
std::string celeritas::concrete_message_handler<Message>::get_supported_type_name() const
{
    return Message::descriptor()->full_name().data();
}

template <typename Message>
bool celeritas::concrete_message_handler<Message>::handle(const handle_parameter& handle_parameter, const google::protobuf::Message& current_message, const message_registry_weak_ptr& message_registry)
{
    const auto& concrete_message = boost::polymorphic_downcast<const Message&>(current_message);

    return handle_concrete(handle_parameter, concrete_message, message_registry);
}

