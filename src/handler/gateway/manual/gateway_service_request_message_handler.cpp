#include "gateway_service_request_message_handler.h"
#include "initializer/initializer_fwd.h"
#include "message/protobuf_handle_parameter.h"

celeritas::gateway_service_request_message_handler::gateway_service_request_message_handler()
{
}

bool celeritas::gateway_service_request_message_handler::handle_concrete(const protobuf_handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    switch (current_message.payload_case())
    {
        case proto::client::client_request::kPlayer:
        {
            handle_parameter.write(player_type.data(), *handle_parameter.get_protobuf_message());
        }
        break;
        default:
        {
            return false;
        }
    }
    return true;
}