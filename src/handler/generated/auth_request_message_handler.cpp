// 此文件是自动生成，请勿手动修改。

#include "auth_request_message_handler.h"
#include "message/concrete_message_handler.tpp"

celeritas::auth_request_message_handler::auth_request_message_handler()
{
    add_handler_function(proto::service::auth_request::PayloadCase::kReloadAppDb,
                         [](const protobuf_handle_parameter& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::reload_app_db);
                         });
    add_handler_function(proto::service::auth_request::PayloadCase::kReloadEmailProvidersDb,
                         [](const protobuf_handle_parameter& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::reload_email_providers_db);
                         });
    add_handler_function(proto::service::auth_request::PayloadCase::kReloadSmsProvidersDb,
                         [](const protobuf_handle_parameter& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::reload_sms_providers_db);
                         });

}

bool celeritas::auth_request_message_handler::handle_concrete(const protobuf_handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    return handle_forward(handle_parameter, current_message, message_registry);
}