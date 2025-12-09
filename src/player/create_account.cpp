#include "create_account.h"

celeritas::create_account::create_account(protobuf_handle_parameter protobuf_handle_parameter, const proto::service::service_login_request& login)
    : protobuf_handle_parameter_{ std::move(protobuf_handle_parameter) }, login_{ login }
{
}

celeritas::create_account::void_awaitable_type celeritas::create_account::send_message() const
{
    co_return;
}