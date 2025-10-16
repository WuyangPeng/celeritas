#include "service_registry_request_message_handler.h"
#include "common/logger.h"
#include "message/concrete_message_handler.tpp"

bool celeritas::service_registry_request_message_handler::handle_concrete(const header& header, const message_type& current_message, const protobuf_message_shared_ptr& request_message, const message_registry_weak_ptr& message_registry, const session_shared_ptr& session)
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
            return handle_server_register(header, current_message, request_message, message_registry_shared_ptr, session);
        }

        case proto::service::service_registry_request::PayloadCase::kServerDiscover:
        {
            return handle_server_discover(header, current_message, request_message, message_registry_shared_ptr, session);
        }

        default:
        {
            LOG_CHANNEL(initializer_channel, error) << "消息体为空.";
            return false;
        }
    }
}

bool celeritas::service_registry_request_message_handler::handle_server_register(const header& header, const message_type& current_message, const protobuf_message_shared_ptr& request_message, const message_registry_shared_ptr& message_registry, const session_shared_ptr& session)
{
    if (const auto& server_register = current_message.server_register();
        !message_registry->dispatch(header, server_register, request_message, session))
    {
        LOG_CHANNEL(initializer_channel, error) << "Failed to dispatch service register request.";
        return false;
    }

    return true;
}

bool celeritas::service_registry_request_message_handler::handle_server_discover(const header& header, const message_type& current_message, const protobuf_message_shared_ptr& request_message, const message_registry_shared_ptr& message_registry, const session_shared_ptr& session)
{
    if (const auto& server_discover = current_message.server_discover();
        !message_registry->dispatch(header, server_discover, request_message, session))
    {
        LOG_CHANNEL(initializer_channel, error) << "Failed to dispatch discover request.";
        return false;
    }
    return true;
}