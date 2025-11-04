#include "health_check.h"
#include "health_check_request_http_message_handler.h"

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
    const auto app_config = handle_parameter.get_app_config();
    const auto instance_id = app_config->get_server_config().get_instance_id();
    const health_check health_check{ instance_id, handle_parameter.get_health_check_level() };
    handle_parameter.write(health_check.to_json_string());
    return true;
}