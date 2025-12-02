#include "sdk_login_http_message_handler.h"
#include "auth/sdk_login.h"
#include "auth/sdk_login_response.h"
#include "common/logger.h"
#include "handler/handler_fwd.h"
#include "message/http_handle_parameter.h"
#include "message/game_error_type.h"
#include "server/server_fwd.h"

std::string celeritas::sdk_login_http_message_handler::get_supported_type_name() const
{
    return sdk_login_path.data();
}

bool celeritas::sdk_login_http_message_handler::handle(const http_handle_parameter& handle_parameter, const http_message_registry_weak_ptr& message_registry)
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

celeritas::sdk_login_http_message_handler::void_awaitable_type celeritas::sdk_login_http_message_handler::response(http_handle_parameter handle_parameter)
{
    try
    {
        sdk_login sdk_login{ std::move(handle_parameter) };
        co_return co_await sdk_login.response();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(handler_channel, error) << "sdk login error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(handler_channel, fatal) << "sdk login unknown error.";
    }

    const sdk_login_response response{ game_error_type::unknown, "unknown error" };
    handle_parameter.write(response.to_json_string());
}