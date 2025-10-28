#include "health_check_http_message_handler.h"

celeritas::health_check_http_message_handler::health_check_http_message_handler(std::string path)
    : path_{ std::move(path) }
{
}

std::string celeritas::health_check_http_message_handler::get_supported_type_name() const
{
    return path_;
}

bool celeritas::health_check_http_message_handler::handle(const http_handle_parameter& handle_parameter, const http_message_registry_weak_ptr& message_registry)
{
    std::cout << "health_check_http_message_handler:" << handle_parameter.get_path() << std::endl;
    return true;
}