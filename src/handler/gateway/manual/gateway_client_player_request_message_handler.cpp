#include "gateway_client_player_request_message_handler.h"
#include "gateway/gateway_login.h"
#include "message/concrete_message_handler.tpp"

celeritas::gateway_client_player_request_message_handler::gateway_client_player_request_message_handler()
{
    add_handler_function(proto::client::client_player_request::PayloadCase::kLogin,
                         [](const protobuf_handle_parameter& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::login);
                         });
}

bool celeritas::gateway_client_player_request_message_handler::handle_concrete(const protobuf_handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    if (!handle_forward(handle_parameter, current_message, message_registry))
    {
        switch (current_message.payload_case())
        {
            // 转发到其他服务器
            default:
            {
                return false;
            }
        }
        return true;
    }
    return true;
}

