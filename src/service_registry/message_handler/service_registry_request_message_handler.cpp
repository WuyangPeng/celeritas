#include "service_registry_request_message_handler.h"
#include "common/logger.h"
#include "message/concrete_message_handler.tpp"

bool celeritas::service_registry_request_message_handler::handle_concrete(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    const auto message_registry_shared_ptr = message_registry.lock();
    if (message_registry_shared_ptr == nullptr)
    {
        return false;
    }

    switch (current_message.payload_case())
    {
        case proto::service::service_registry_request::PayloadCase::kServerRegister:
        {
            return handle_server_register(handle_parameter, current_message, message_registry_shared_ptr);
        }

        case proto::service::service_registry_request::PayloadCase::kServerDiscover:
        {
            return handle_server_discover(handle_parameter, current_message, message_registry_shared_ptr);
        }

        default:
        {
            LOG_CHANNEL(initializer_channel, error) << "消息体为空.";
            return false;
        }
    }
}

bool celeritas::service_registry_request_message_handler::handle_server_register(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_shared_ptr& message_registry)
{
    if (const auto& server_register = current_message.server_register();
        !message_registry->dispatch(handle_parameter, server_register))
    {
        LOG_CHANNEL(initializer_channel, error) << "Failed to dispatch server register request.";
        return false;
    }

    return true;
}

bool celeritas::service_registry_request_message_handler::handle_server_discover(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_shared_ptr& message_registry)
{
    if (const auto& server_discover = current_message.server_discover();
        !message_registry->dispatch(handle_parameter, server_discover))
    {
        LOG_CHANNEL(initializer_channel, error) << "Failed to dispatch server discover request.";
        return false;
    }
    return true;
}