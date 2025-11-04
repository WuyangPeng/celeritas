#include "health_check.h"
#include "health_check_request_http_message_handler.h"
#include "common/logger.h"

celeritas::health_check_request_http_message_handler::health_check_request_http_message_handler(std::string path)
    : path_{ std::move(path) }
{
}

std::string celeritas::health_check_request_http_message_handler::get_supported_type_name() const
{
    return path_;
}

bool celeritas::health_check_request_http_message_handler::handle(const http_handle_parameter& handle_parameter,
                                                                  const http_message_registry_weak_ptr& message_registry)
{
    co_spawn(handle_parameter.get_io_context(),
             health_check_result(handle_parameter),
             boost::asio::detached);

    return true;
}

celeritas::health_check_request_http_message_handler::void_awaitable_type celeritas::health_check_request_http_message_handler::health_check_result(const http_handle_parameter& handle_parameter)
{
    try
    {
        co_await do_health_check_result(handle_parameter);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(message_channel, error) << "health check error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(message_channel, fatal) << "health check unknown error.";
    }
}

celeritas::health_check_request_http_message_handler::void_awaitable_type celeritas::health_check_request_http_message_handler::do_health_check_result(const http_handle_parameter& handle_parameter)
{
    const auto app_config = handle_parameter.get_app_config();
    const auto instance_id = app_config->get_server_config().get_instance_id();

    const auto health_check_level = co_await handle_parameter.get_health_check_level();

    const health_check health_check{ instance_id, health_check_level };
    handle_parameter.write(health_check.to_json_string());

    co_return;
}