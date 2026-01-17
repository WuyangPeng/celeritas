#include "health_check_response_http_message_handler.h"
#include "network/session_helper/detail/network_session_helper_internal_constant.h"
#include "message/parameters/http_handle_parameter.h"
#include "service_registry/core/service_registry.h"
#include "service_registry/data/health_check.h"

celeritas::health_check_response_http_message_handler::health_check_response_http_message_handler(std::string path)
    : path_{ std::move(path) }
{
}

std::string celeritas::health_check_response_http_message_handler::get_supported_type_name() const
{
    return path_ + response_suffix.data();
}

bool celeritas::health_check_response_http_message_handler::handle(const http_handle_parameter_shared_ptr& handle_parameter,
                                                                   const http_message_registry_weak_ptr& message_registry)
{
    const auto health_check = health_check::from_json_string(handle_parameter->get_response());

    service_registry::set_service_health(health_check.get_instance_id(), health_check.get_health_check_level());

    return true;
}