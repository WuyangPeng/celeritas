#include "service_registry_request_message_handler.h"
#include "message/concrete_message_handler.tpp"
#include "common/logger.h"
#include "common/common_fwd.h"

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
            const auto& service = message->register_();
            const auto register_request = std::make_shared<proto::service::register_request>();
            register_request->CopyFrom(service);

            if (!message_registry_shared_ptr->dispatch(header, register_request))
            {
                LOG_CHANNEL(initializer_channel, error) << "Failed to dispatch service register request.";
                return false;
            }
            return true;
        }

        case proto::service::service_registry_request::PayloadCase::kDiscover:
        {
            const auto& service = message->discover();
            const auto discover_request = std::make_shared<proto::service::discover_request>();
            discover_request->CopyFrom(service);

            if (!message_registry_shared_ptr->dispatch(header, discover_request))
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