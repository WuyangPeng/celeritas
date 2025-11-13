// 此文件是自动生成，请勿手动修改。

#include "service_registry_response_message_handler.h"
#include "message/concrete_message_handler.tpp"

celeritas::service_registry_response_message_handler::service_registry_response_message_handler()
{
    add_handler_function(proto::service::service_registry_response::PayloadCase::kServerDiscover,
                         [](const handle_parameter& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::server_discover);
                         });
    add_handler_function(proto::service::service_registry_response::PayloadCase::kServerRegister,
                         [](const handle_parameter& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::server_register);
                         });

}

bool celeritas::service_registry_response_message_handler::handle_concrete(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    return handle_forward(handle_parameter, current_message, message_registry);
}