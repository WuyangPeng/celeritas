#include "send_sms_http_message_handler.h"
#include "auth/authentication/phone/send_sms.h"
#include "auth/authentication/phone/send_sms_response.h"
#include "boost/asio/co_spawn.hpp"
#include "common/logger.h"
#include "handler/handler_fwd.h"
#include "initializer/initializer_fwd.h"
#include "message/game_error_type.h"
#include "message/http_handle_parameter.h"

std::string celeritas::send_sms_http_message_handler::get_supported_type_name() const
{
    return send_sms_path.data();
}

bool celeritas::send_sms_http_message_handler::handle(const http_handle_parameter& handle_parameter,
                                                      const http_message_registry_weak_ptr& message_registry)
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

celeritas::send_sms_http_message_handler::void_awaitable_type celeritas::send_sms_http_message_handler::response(http_handle_parameter handle_parameter)
{
    try
    {
        send_sms send_sms{ handle_parameter };
        co_return co_await send_sms.response();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(handler_channel, error) << "send sms error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(handler_channel, fatal) << "send sms unknown error.";
    }

    const send_sms_response response{ game_error_type::unknown, "unknown error" };
    co_return co_await handle_parameter.write_immediately(response.to_json_string());
}