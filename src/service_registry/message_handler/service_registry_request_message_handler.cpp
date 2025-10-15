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
        case proto::service::service_registry_request::PayloadCase::kRegister:
        {
            if (const auto& service = message->register_();
                !dispatch<proto::service::register_request>(header, service, message_registry_shared_ptr))
            {
                LOG_CHANNEL(initializer_channel, error) << "Failed to dispatch service register request.";
                return false;
            }
            return true;
        }

        case proto::service::service_registry_request::PayloadCase::kDiscover:
        {
            if (const auto& discover = message->discover();
                !dispatch<proto::service::discover_request>(header, discover, message_registry_shared_ptr))
            {
                LOG_CHANNEL(initializer_channel, error) << "Failed to dispatch discover request.";
                return false;
            }
            return true;
        }

        default:
        {
            LOG_CHANNEL(initializer_channel, error) << "消息体为空.";
            return false;
        }
    }
}