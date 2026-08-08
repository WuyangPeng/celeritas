#include "announcement_admin_update_http_message_handler.h"
#include "admin/announcement/announcement_admin_update.h"
#include "common/logging/logger.h"
#include "handler/handler_constant.h"
#include "initializer/initializer_constant.h"
#include "message/handler_base/http_base_message_handler.tpp"

std::string celeritas::announcement_admin_update_http_message_handler::get_supported_type_name() const
{
    return announcement_admin_update_path.data();
}

bool celeritas::announcement_admin_update_http_message_handler::handle(const http_handle_parameter_shared_ptr& handle_parameter,
                                                                       const http_message_registry_weak_ptr& message_registry)
{
    co_spawn_response<announcement_admin_update>(handle_parameter, handler_channel, "announcement admin update error:");
    return true;
}

std::string celeritas::announcement_admin_update_http_message_handler::get_server_type() const
{
    return admin_type.data();
}
