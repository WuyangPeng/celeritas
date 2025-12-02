// 此文件是自动生成，请勿手动修改。

#include "payment_request_message_handler.h"
#include "message/concrete_message_handler.tpp"

celeritas::payment_request_message_handler::payment_request_message_handler()
{
    add_handler_function(proto::service::payment_request::PayloadCase::kReloadSdkPaymentProvidersDb,
                         [](const protobuf_handle_parameter& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::reload_sdk_payment_providers_db);
                         });

}

bool celeritas::payment_request_message_handler::handle_concrete(const protobuf_handle_parameter& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    return handle_forward(handle_parameter, current_message, message_registry);
}