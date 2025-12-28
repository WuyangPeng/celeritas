#include "service_login_request_message_handler.h"
#include "message/concrete_message_handler.tpp"
#include "message/protobuf_handle_parameter.h"
#include "player/login/service_login.h"

celeritas::service_login_request_message_handler::service_login_request_message_handler()
{
}

bool celeritas::service_login_request_message_handler::handle_concrete(const protobuf_handle_parameter_shared_ptr& handle_parameter, const message_type& current_message, const message_registry_weak_ptr& message_registry)
{
    boost::asio::co_spawn(handle_parameter->get_io_context(),
                          response(handle_parameter, current_message),
                          boost::asio::detached);
    return true;
}

celeritas::service_login_request_message_handler::void_awaitable_type celeritas::service_login_request_message_handler::response(protobuf_handle_parameter_shared_ptr protobuf_handle_parameter, const message_type& login)
{
    try
    {
        const service_login service_login{ std::move(protobuf_handle_parameter), login };

        co_await service_login.send_message();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(message_channel, error) << "service login error " << error.what();
    }

    co_return;
}