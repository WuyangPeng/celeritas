#include "email_bind_http_message_handler.h"
#include "auth/authentication/email/email_bind_response.h"
#include "auth/authentication/email/email_bind.h"
#include "common/logging/logger.h"
#include "handler/handler_fwd.h"
#include "initializer/initializer_constant.h"
#include "message/parameters/http_handle_parameter.h"
#include "message/basic/game_error_type.h"
#include "initializer/initializer_fwd.h"

std::string celeritas::email_bind_http_message_handler::get_supported_type_name() const
{
    return email_bind_path.data();
}

bool celeritas::email_bind_http_message_handler::handle(const http_handle_parameter_shared_ptr& handle_parameter,
                                                        const http_message_registry_weak_ptr& message_registry)
{
    if (handle_parameter->get_server_type() != auth_type)
    {
        return false;
    }

    boost::asio::co_spawn(handle_parameter->get_any_io_executor(),
                          response(handle_parameter),
                          boost::asio::detached);

    return true;
}

celeritas::email_bind_http_message_handler::void_awaitable_type celeritas::email_bind_http_message_handler::response(http_handle_parameter_shared_ptr handle_parameter)
{
    try
    {
        email_bind email_bind{ handle_parameter };
        co_return co_await email_bind.response();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(handler_channel, error) << "phone login error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(handler_channel, fatal) << "phone login unknown error.";
    }

    const email_bind_response response{ game_error_type::unknown, "unknown error" };
    co_return co_await handle_parameter->write_immediately(response.to_json_string());
}