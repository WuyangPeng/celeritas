#include "service_request_message_handler.h"
#include "concrete_message_handler.tpp"
#include "common/logger.h"
#include "common/common_fwd.h"

bool celeritas::service_request_message_handler::handle_concrete(const header& header, const message_type& current_message, const protobuf_message_shared_ptr& request_message, const message_registry_weak_ptr& message_registry)
{
    const auto message_registry_shared_ptr = message_registry.lock();
    if (message_registry_shared_ptr == nullptr)
    {
        return false;
    }

    switch (current_message.payload_case())
    {
        case proto::service::service_request::PayloadCase::kRegistry:
        {
            const auto& service = current_message.registry();

            if (!message_registry_shared_ptr->dispatch(header, service, request_message))
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