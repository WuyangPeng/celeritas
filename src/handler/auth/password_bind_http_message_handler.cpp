#include "password_bind_http_message_handler.h"
#include "auth/authentication/password/password_bind.h"
#include "auth/authentication/password/password_bind_response.h"
#include "auth/authentication/phone/phone_bind.h"
#include "common/logging/logger.h"
#include "handler/handler_fwd.h"
#include "initializer/initializer_fwd.h"
#include "message/game_error_type.h"
#include "message/http_handle_parameter.h"

std::string celeritas::password_bind_http_message_handler::get_supported_type_name() const
{
    return password_bind_path.data();
}

bool celeritas::password_bind_http_message_handler::handle(const http_handle_parameter_shared_ptr& handle_parameter,
                                                           const http_message_registry_weak_ptr& message_registry)
{
    if (handle_parameter->get_server_type() != auth_type)
    {
        return false;
    }

    boost::asio::co_spawn(handle_parameter->get_io_context(),
                          response(handle_parameter),
                          boost::asio::detached);

    return true;
}

celeritas::password_bind_http_message_handler::void_awaitable_type celeritas::password_bind_http_message_handler::response(http_handle_parameter_shared_ptr handle_parameter)
{
    try
    {
        password_bind password_bind{ handle_parameter };
        co_return co_await password_bind.response();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(handler_channel, error) << "phone login error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(handler_channel, fatal) << "phone login unknown error.";
    }

    const password_bind_response response{ game_error_type::unknown, "unknown error" };
    co_return co_await handle_parameter->write_immediately(response.to_json_string());
}