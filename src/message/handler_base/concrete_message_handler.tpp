#pragma once

#include "concrete_message_handler.h"
#include "common/core/celeritas_error.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "common/logging/logger.h"
#include "message/parameters/protobuf_handle_parameter.h"
#include "message/registry/protobuf_message_registry.h"

#include <boost/polymorphic_cast.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/co_spawn.hpp>

template <typename Message>
std::string celeritas::concrete_message_handler<Message>::get_supported_type_name() const
{
    return Message::descriptor()->full_name().data();
}

template <typename Message>
bool celeritas::concrete_message_handler<Message>::handle(const protobuf_handle_parameter_shared_ptr& handle_parameter,
                                                          const protobuf_message& current_message,
                                                          const message_registry_weak_ptr& message_registry)
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
bool celeritas::concrete_message_handler<Message>::has_handler_function(int payload_case) const
{
    return handler_.contains(payload_case);
}

template <typename Message>
bool celeritas::concrete_message_handler<Message>::handle_forward(const protobuf_handle_parameter_shared_ptr& handle_parameter,
                                                                  const message_type& current_message,
                                                                  const message_registry_weak_ptr& message_registry)
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
bool celeritas::concrete_message_handler<Message>::has_handle(const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    if (const auto message_registry_shared_ptr = message_registry.lock();
        message_registry_shared_ptr != nullptr)
    {
        return has_handler_function(current_message.payload_case());
    }

    return false;
}

template <typename Message>
template <typename GetFunction>
bool celeritas::concrete_message_handler<Message>::handle_dispatch(const protobuf_handle_parameter_shared_ptr& handle_parameter,
                                                                   const message_type& current_message,
                                                                   const message_registry_shared_ptr& message_registry,
                                                                   GetFunction get_function)
{
    if (const auto& result = (current_message.*get_function)();
        !message_registry->dispatch(handle_parameter, result))
    {
        LOG_CHANNEL(message_channel, error) << "failed to dispatch message.";
        return false;
    }

    return true;
}

template <typename Message>
template <typename ServiceType, typename... Args>
void celeritas::concrete_message_handler<Message>::co_spawn_response(protobuf_handle_parameter_shared_ptr handle_parameter, 
                                                                      const message_type& current_message, 
                                                                      const std::string_view channel_name, 
                                                                      std::format_string<Args...> format,
                                                                      Args... args)
{
    safe_co_spawn(handle_parameter->get_any_io_executor(),
                  [handle_parameter, current_message, channel_name, format, args...] {
                      return response<ServiceType>(handle_parameter, current_message, channel_name, format, args...);
                  },
                  channel_name,
                  format,
                  args...);
}

template <typename Message>
template <typename ServiceType, typename... Args>
celeritas::concrete_message_handler<Message>::void_awaitable_type celeritas::concrete_message_handler<Message>::response(protobuf_handle_parameter_shared_ptr handle_parameter, 
                                                                                                                          const message_type& current_message, 
                                                                                                                          const std::string_view channel_name, 
                                                                                                                          std::format_string<Args...> format,
                                                                                                                          Args... args)
{
    if (auto service = std::make_shared<ServiceType>(std::move(handle_parameter), current_message);
        !co_await noexcept_safe_call_and_log_awaitable([service = service]() -> boost::asio::awaitable<bool> {
                                                           co_await service->response();
                                                           co_return true;
                                                       },
                                                       channel_name,
                                                       false,
                                                       format,
                                                       args...))
    {
        service->send_error_message(game_error_type::unknown);
    }
}

