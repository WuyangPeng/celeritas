#include "announcement_red_dot_http_message_handler.h"
#include "auth/announcement/announcement_red_dot.h"
#include "common/logging/logger.h"
#include "handler/handler_constant.h"
#include "initializer/initializer_constant.h"
#include "message/handler_base/http_base_message_handler.tpp"

std::string celeritas::announcement_red_dot_http_message_handler::get_supported_type_name() const
{
    return announcement_red_dot_path.data();
}

bool celeritas::announcement_red_dot_http_message_handler::handle(const http_handle_parameter_shared_ptr& handle_parameter,
                                                                 const http_message_registry_weak_ptr& message_registry)
{
    co_spawn_response<announcement_red_dot>(handle_parameter, handler_channel, "announcement red dot error:");
    return true;
}

std::string celeritas::announcement_red_dot_http_message_handler::get_server_type() const
{
    return auth_type.data();
}
