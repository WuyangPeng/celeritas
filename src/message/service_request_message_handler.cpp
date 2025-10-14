#include "service_request_message_handler.h"
#include "concrete_message_handler.tpp"
#include "common/logger.h"
#include "common/common_fwd.h"

bool celeritas::service_request_message_handler::handle_concrete(const header& header, const message_shared_ptr& message, const message_registry_weak_ptr& message_registry)
{
    const auto message_registry_shared_ptr = message_registry.lock();
    if (message_registry_shared_ptr == nullptr)
    {
        return false;
    }

    switch (message->payload_case())
    {
        case proto::service::service_request::PayloadCase::kRegistry:
        {
            const auto& service = message->registry();
            const auto service_registry_request = std::make_shared<proto::service::service_registry_request>();
            service_registry_request->CopyFrom(service);

            if (!message_registry_shared_ptr->dispatch(header, service_registry_request))
            {
                LOG_CHANNEL(initializer_channel, error) << "Failed to dispatch service request.";
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