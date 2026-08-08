#include "admin_create_http_message_handler.h"
#include "admin/account/admin_create.h"
#include "common/logging/logger.h"
#include "handler/handler_constant.h"
#include "initializer/initializer_constant.h"
#include "message/handler_base/http_base_message_handler.tpp"

std::string celeritas::admin_create_http_message_handler::get_supported_type_name() const
{
    return admin_create_path.data();
}

bool celeritas::admin_create_http_message_handler::handle(const http_handle_parameter_shared_ptr& handle_parameter,
                                                          const http_message_registry_weak_ptr& message_registry)
{
    co_spawn_response<admin_create>(handle_parameter, handler_channel, "admin create error:");
    return true;
}

std::string celeritas::admin_create_http_message_handler::get_server_type() const
{
    return admin_type.data();
}
