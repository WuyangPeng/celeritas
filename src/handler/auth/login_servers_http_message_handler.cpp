#include "login_servers_http_message_handler.h"
#include "auth/server_list/login_servers.h"
#include "common/core/celeritas_error.h"
#include "common/logging/logger.h"
#include "handler/handler_fwd.h"
#include "initializer/initializer_constant.h"
#include "message/handler_base/http_base_message_handler.tpp"

std::string celeritas::login_servers_http_message_handler::get_supported_type_name() const
{
    return login_servers_path.data();
}

bool celeritas::login_servers_http_message_handler::handle(const http_handle_parameter_shared_ptr& handle_parameter,
                                                           const http_message_registry_weak_ptr& message_registry)
{
    co_spawn_response<login_servers>(handle_parameter, handler_channel, "login servers error:");

    return true;
}

std::string celeritas::login_servers_http_message_handler::get_server_type() const
{
    return auth_type.data();
}
