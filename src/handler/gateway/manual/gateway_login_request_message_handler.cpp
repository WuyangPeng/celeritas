#include "gateway_login_request_message_handler.h"
#include "gateway/gateway_login.h"
#include "message/concrete_message_handler.tpp"
#include "message/protobuf_handle_parameter.h"

celeritas::gateway_login_request_message_handler::gateway_login_request_message_handler()
{
}

bool celeritas::gateway_login_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    boost::asio::co_spawn(handle_parameter->get_io_context(),
                          response(handle_parameter, current_message),
                          boost::asio::detached);
    return true;
}

celeritas::gateway_login_request_message_handler::void_awaitable_type celeritas::gateway_login_request_message_handler::response(protobuf_handle_parameter_shared_ptr protobuf_handle_parameter, const message_type& login)
{
    const gateway_login gateway_login{ std::move(protobuf_handle_parameter), login };

    co_return co_await gateway_login.send_message();
}