// 此文件是自动生成，请勿手动修改。

#include "service_registry_request_message_handler.h"
#include "message/concrete_message_handler.tpp"

celeritas::service_registry_request_message_handler::service_registry_request_message_handler()
{
    add_handler_function(proto::service::service_registry_request::PayloadCase::kServerClose,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::server_close);
                         });
    add_handler_function(proto::service::service_registry_request::PayloadCase::kServerDiscover,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::server_discover);
                         });
    add_handler_function(proto::service::service_registry_request::PayloadCase::kServerRegister,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::server_register);
                         });
    add_handler_function(proto::service::service_registry_request::PayloadCase::kServiceHeartbeat,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::service_heartbeat);
                         });

}

bool celeritas::service_registry_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    return handle_forward(handle_parameter, current_message, message_registry);
}