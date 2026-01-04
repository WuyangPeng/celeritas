// 此文件是自动生成，请勿手动修改。

#include "service_auth_request_message_handler.h"
#include "message/handler_base/concrete_message_handler.tpp"

celeritas::service_auth_request_message_handler::service_auth_request_message_handler()
{
    add_handler_function(proto::service::service_auth_request::PayloadCase::kReloadAppDb,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::reload_app_db);
                         });
    add_handler_function(proto::service::service_auth_request::PayloadCase::kReloadEmailProvidersDb,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::reload_email_providers_db);
                         });
    add_handler_function(proto::service::service_auth_request::PayloadCase::kReloadSdkProvidersDb,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::reload_sdk_providers_db);
                         });
    add_handler_function(proto::service::service_auth_request::PayloadCase::kReloadServerCellDb,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::reload_server_cell_db);
                         });
    add_handler_function(proto::service::service_auth_request::PayloadCase::kReloadSmsProvidersDb,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::reload_sms_providers_db);
                         });

}

bool celeritas::service_auth_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    return handle_forward(handle_parameter, current_message, message_registry);
}