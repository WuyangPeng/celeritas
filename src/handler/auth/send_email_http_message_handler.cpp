#include "send_email_http_message_handler.h"
#include "auth/send_email.h"
#include "auth/send_email_response.h"
#include "boost/asio/co_spawn.hpp"
#include "common/logger.h"
#include "handler/handler_fwd.h"
#include "message/http_handle_parameter.h"
#include "../../message/game_error_type.h"
#include "server/server_fwd.h"

std::string celeritas::send_email_http_message_handler::get_supported_type_name() const
{
    return send_email_path.data();
}

bool celeritas::send_email_http_message_handler::handle(const http_handle_parameter& handle_parameter, const http_message_registry_weak_ptr& message_registry)
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

celeritas::send_email_http_message_handler::void_awaitable_type celeritas::send_email_http_message_handler::response(http_handle_parameter handle_parameter)
{
    try
    {
        send_email send_email{ std::move(handle_parameter) };

        co_return co_await send_email.response();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(handler_channel, error) << "send sms error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(handler_channel, fatal) << "send sms unknown error.";
    }

    const send_email_response response{ game_error_type::unknown, "unknown error" };
    handle_parameter.write(response.to_json_string());
}