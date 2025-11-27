#include "email_login_http_message_handler.h"
#include "auth/email_login.h"
#include "auth/email_login_response.h"
#include "auth/phone_login.h"
#include "common/logger.h"
#include "handler/handler_fwd.h"
#include "message/http_handle_parameter.h"
#include "../../message/game_error_type.h"
#include "server/server_fwd.h"

std::string celeritas::email_login_http_message_handler::get_supported_type_name() const
{
    return email_login_path.data();
}

bool celeritas::email_login_http_message_handler::handle(const http_handle_parameter& handle_parameter, const http_message_registry_weak_ptr& message_registry)
{
    if (handle_parameter.get_server_type() != auth_type)
    {
        return false;
    }

    boost::asio::co_spawn(handle_parameter.get_io_context(),
                          response(handle_parameter),
                          boost::asio::detached);

    return true;
}

celeritas::email_login_http_message_handler::void_awaitable_type celeritas::email_login_http_message_handler::response(http_handle_parameter handle_parameter)
{
    try
    {
        email_login email_login{ std::move(handle_parameter) };
        co_return co_await email_login.response();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(handler_channel, error) << "phone login error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(handler_channel, fatal) << "phone login unknown error.";
    }

    const email_login_response response{ game_error_type::unknown, "unknown error" };
    handle_parameter.write(response.to_json_string());
}