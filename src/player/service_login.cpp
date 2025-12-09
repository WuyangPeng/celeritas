#include "create_account.h"
#include "service_login.h"

celeritas::service_login::service_login(protobuf_handle_parameter protobuf_handle_parameter, const proto::service::service_login_request& login)
    : protobuf_handle_parameter_{ std::move(protobuf_handle_parameter) }, login_{ login }
{
}

celeritas::service_login::void_awaitable_type celeritas::service_login::send_message() const
{
    if (login_.new_account())
    {
        const create_account create_account{ protobuf_handle_parameter_, login_ };
        co_await create_account.send_message();
    }

    co_return;
}