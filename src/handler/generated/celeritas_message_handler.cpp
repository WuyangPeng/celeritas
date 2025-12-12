// 此文件是自动生成，请勿手动修改。

#include "celeritas_message_handler.h"
#include "message/concrete_message_handler.tpp"

celeritas::celeritas_message_handler::celeritas_message_handler()
{
    add_handler_function(proto::celeritas::PayloadCase::kCeleritasRequest,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::celeritas_request);
                         });
    add_handler_function(proto::celeritas::PayloadCase::kCeleritasResponse,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::celeritas_response);
                         });

}

bool celeritas::celeritas_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    return handle_forward(handle_parameter, current_message, message_registry);
}