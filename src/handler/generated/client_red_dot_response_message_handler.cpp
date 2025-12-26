// 此文件是自动生成，请勿手动修改。

#include "client_red_dot_response_message_handler.h"
#include "message/concrete_message_handler.tpp"

celeritas::client_red_dot_response_message_handler::client_red_dot_response_message_handler()
{
    add_handler_function(proto::client::client_red_dot_response::PayloadCase::kRedDot,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::red_dot);
                         });

}

bool celeritas::client_red_dot_response_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    return handle_forward(handle_parameter, current_message, message_registry);
}