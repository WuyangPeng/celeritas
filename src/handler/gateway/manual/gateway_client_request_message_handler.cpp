#include "gateway_client_request_message_handler.h"
#include "message/protobuf_handle_parameter.h"
#include "message/concrete_message_handler.tpp"

celeritas::gateway_client_request_message_handler::gateway_client_request_message_handler()
{
    add_handler_function(proto::client::client_request::PayloadCase::kPlayer,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::player);
                         });
}

bool celeritas::gateway_client_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    if (has_handle(current_message, message_registry))
    {
        return handle_forward(handle_parameter, current_message, message_registry);
    }

    switch (current_message.payload_case())
    {
        case proto::client::client_request::PayloadCase::PAYLOAD_NOT_SET:
        {
            return false;
        }
        // 转发到其他服务器
        default:
        {
            return true;
        }
    }
}