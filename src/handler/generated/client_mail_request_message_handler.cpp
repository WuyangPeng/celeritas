// 此文件是自动生成，请勿手动修改。

#include "client_mail_request_message_handler.h"
#include "message/handler_base/concrete_message_handler.tpp"

celeritas::client_mail_request_message_handler::client_mail_request_message_handler()
{
    add_handler_function(proto::client::client_mail_request::PayloadCase::kMail,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::mail);
                         });
    add_handler_function(proto::client::client_mail_request::PayloadCase::kMailCollectAllAttachments,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::mail_collect_all_attachments);
                         });
    add_handler_function(proto::client::client_mail_request::PayloadCase::kMailCollectAttachment,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::mail_collect_attachment);
                         });
    add_handler_function(proto::client::client_mail_request::PayloadCase::kMailDelete,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::mail_delete);
                         });
    add_handler_function(proto::client::client_mail_request::PayloadCase::kMailDeleteAllRead,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::mail_delete_all_read);
                         });
    add_handler_function(proto::client::client_mail_request::PayloadCase::kMailRead,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::mail_read);
                         });

}

bool celeritas::client_mail_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    return handle_forward(handle_parameter, current_message, message_registry);
}