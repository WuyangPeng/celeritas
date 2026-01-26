#include "health_check_request_http_message_handler.h"
#include "common/logging/logger.h"
#include "config/aggregate/app_config.h"
#include "message/parameters/http_handle_parameter.h"
#include "service_registry/data/health_check.h"

celeritas::health_check_request_http_message_handler::health_check_request_http_message_handler(std::string path)
    : path_{ std::move(path) }
{
}

std::string celeritas::health_check_request_http_message_handler::get_supported_type_name() const
{
    return path_;
}

bool celeritas::health_check_request_http_message_handler::handle(const http_handle_parameter_shared_ptr& handle_parameter,
                                                                  const http_message_registry_weak_ptr& message_registry)
{
    co_spawn(handle_parameter->get_any_io_executor(),
             noexcept_safe_call_and_log_awaitable([handle_parameter = handle_parameter] {
                                                      return health_check_result(handle_parameter);
                                                  },
                                                  handler_channel,
                                                  "health check error error:"),

             boost::asio::detached);

    return true;
}

celeritas::health_check_request_http_message_handler::void_awaitable_type celeritas::health_check_request_http_message_handler::health_check_result(http_handle_parameter_shared_ptr handle_parameter)
{
    const auto app_config = handle_parameter->get_app_config();
    const auto instance_id = app_config->get_server_config()->get_instance_id();

    const auto health_check_level = co_await handle_parameter->get_health_check_level();

    const health_check health_check{ instance_id, health_check_level };
    co_return co_await handle_parameter->write_immediately(health_check.to_json_string());
}