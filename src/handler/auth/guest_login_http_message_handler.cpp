#include "guest_login_http_message_handler.h"
#include "auth/app_secret.h"
#include "auth/guest_login.h"
#include "auth/guest_login_response.h"
#include "common/celeritas_error.h"
#include "common/logger.h"
#include "config/app_config.h"
#include "database/database_pool_manager.h"
#include "database/mysql_database_session.h"
#include "database/generated/mysql/auth/account.h"
#include "handler/handler_fwd.h"
#include "initializer/initializer_fwd.h"
#include "message/game_error_type.h"
#include "message/http_handle_parameter.h"

std::string celeritas::guest_login_http_message_handler::get_supported_type_name() const
{
    return guest_login_path.data();
}

bool celeritas::guest_login_http_message_handler::handle(const http_handle_parameter& handle_parameter,
                                                         const http_message_registry_weak_ptr& message_registry)
{
    if (handle_parameter.get_server_type() != auth_type)
    {
        return false;
    }

    co_spawn(handle_parameter.get_io_context(),
             response(handle_parameter),
             boost::asio::detached);

    return true;
}

celeritas::guest_login_http_message_handler::void_awaitable_type celeritas::guest_login_http_message_handler::response(http_handle_parameter handle_parameter)
{
    try
    {
        guest_login guest_login{ std::move(handle_parameter) };
        co_return co_await guest_login.response();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(handler_channel, error) << "guest login error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(handler_channel, fatal) << "guest login unknown error.";
    }

    const guest_login_response response{ game_error_type::unknown, "unknown error" };
    handle_parameter.write(response.to_json_string());
}