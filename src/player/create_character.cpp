#include "create_character.h"

celeritas::create_character::create_character(protobuf_handle_parameter protobuf_handle_parameter, const proto::service::service_login_request& login)
    : protobuf_handle_parameter_{ std::move(protobuf_handle_parameter) }, login_{ login }
{
}

celeritas::create_character::void_awaitable_type celeritas::create_character::send_message() const
{
    co_return;
}