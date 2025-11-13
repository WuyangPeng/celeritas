// 此文件是自动生成，请勿手动修改。

#include "service_request_message_handler.h"
#include "message/concrete_message_handler.tpp"

celeritas::service_request_message_handler::service_request_message_handler()
{
    add_handler_function(proto::service::service_request::PayloadCase::kRegistry,
                         [](const handle_parameter& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::registry);
                         });

}

bool celeritas::service_request_message_handler::handle_concrete(const handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    return handle_forward(handle_parameter, current_message, message_registry);
}