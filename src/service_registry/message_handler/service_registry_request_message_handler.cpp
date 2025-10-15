#include "service_registry_request_message_handler.h"
#include "common/logger.h"
#include "message/concrete_message_handler.tpp"

bool celeritas::service_registry_request_message_handler::handle_concrete(const header& header, const message_shared_ptr& message, const message_registry_weak_ptr& message_registry)
{
    const auto message_registry_shared_ptr = message_registry.lock();
    if (message_registry_shared_ptr == nullptr)
    {
        return false;
    }

    switch (message->payload_case())
    {
        case proto::service::service_registry_request::PayloadCase::kServerRegister:
        {
            return handle_server_register(header, message, message_registry_shared_ptr);
        }

        case proto::service::service_registry_request::PayloadCase::kServerDiscover:
        {
            return handle_server_discover(header, message, message_registry_shared_ptr);
        }

        default:
        {
            LOG_CHANNEL(initializer_channel, error) << "消息体为空.";
            return false;
        }
    }
}

bool celeritas::service_registry_request_message_handler::handle_server_register(const header& header, const message_shared_ptr& message, const message_registry_shared_ptr& message_registry)
{
    if (const auto& server_register = message->server_register();
        !dispatch<proto::service::register_request>(header, server_register, message_registry))
    {
        LOG_CHANNEL(initializer_channel, error) << "Failed to dispatch service register request.";
        return false;
    }

    return true;
}

bool celeritas::service_registry_request_message_handler::handle_server_discover(const header& header, const message_shared_ptr& message, const message_registry_shared_ptr& message_registry)
{
    if (const auto& server_discover = message->server_discover();
        !dispatch<proto::service::discover_request>(header, server_discover, message_registry))
    {
        LOG_CHANNEL(initializer_channel, error) << "Failed to dispatch discover request.";
        return false;
    }
    return true;
}