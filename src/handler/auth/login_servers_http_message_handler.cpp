#include "login_servers_http_message_handler.h"
#include "auth/data/app_secret.h"
#include "auth/server_list/login_servers.h"
#include "auth/server_list/login_servers_response.h"
#include "common/celeritas_error.h"
#include "common/logger.h"
#include "config/app_config.h"
#include "handler/handler_fwd.h"
#include "initializer/initializer_fwd.h"
#include "message/game_error_type.h"
#include "message/http_handle_parameter.h"

std::string celeritas::login_servers_http_message_handler::get_supported_type_name() const
{
    return login_servers_path.data();
}

bool celeritas::login_servers_http_message_handler::handle(const http_handle_parameter& handle_parameter,
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

celeritas::login_servers_http_message_handler::void_awaitable_type celeritas::login_servers_http_message_handler::response(http_handle_parameter handle_parameter)
{
    try
    {
        login_servers login_servers{ std::move(handle_parameter) };
        co_return co_await login_servers.response();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(handler_channel, error) << "login servers error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(handler_channel, fatal) << "login servers unknown error.";
    }

    const login_servers_response response{ game_error_type::unknown, "unknown error" };
    co_return co_await handle_parameter.write_immediately(response.to_json_string());
}