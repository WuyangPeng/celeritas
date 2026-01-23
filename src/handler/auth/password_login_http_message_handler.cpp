#include "password_login_http_message_handler.h"
#include "auth/password/password_login.h"
#include "common/logging/logger.h"
#include "handler/handler_fwd.h"
#include "initializer/initializer_constant.h"

std::string celeritas::password_login_http_message_handler::get_supported_type_name() const
{
    return password_login_path.data();
}

bool celeritas::password_login_http_message_handler::handle(const http_handle_parameter_shared_ptr& handle_parameter,
                                                            const http_message_registry_weak_ptr& message_registry)
{
    co_spawn_response<password_login>(handle_parameter, handler_channel, "password login error:");

    return true;
}

std::string celeritas::password_login_http_message_handler::get_server_type() const
{
    return auth_type.data();
}
