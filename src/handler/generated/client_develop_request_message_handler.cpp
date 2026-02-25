// 此文件是自动生成，请勿手动修改。

#include "client_develop_request_message_handler.h"
#include "message/handler_base/concrete_message_handler.tpp"

celeritas::client_develop_request_message_handler::client_develop_request_message_handler()
{
    add_handler_function(proto::client::client_develop_request::PayloadCase::kDevelopClaimReward,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::develop_claim_reward);
                         });
    add_handler_function(proto::client::client_develop_request::PayloadCase::kDevelopLevel,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::develop_level);
                         });
    add_handler_function(proto::client::client_develop_request::PayloadCase::kDevelopReset,
                         [](const protobuf_handle_parameter_shared_ptr& handle_parameter,
                            const message_type& current_message,
                            const message_registry_shared_ptr& message_registry) -> bool {
                             return handle_dispatch(handle_parameter, current_message, message_registry, &message_type::develop_reset);
                         });

}

bool celeritas::client_develop_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    return handle_forward(handle_parameter, current_message, message_registry);
}