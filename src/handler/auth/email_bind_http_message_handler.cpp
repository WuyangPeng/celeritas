#include "email_bind_http_message_handler.h"
#include "auth/email/email_bind.h"
#include "common/logging/logger.h"
#include "handler/handler_constant.h"
#include "initializer/initializer_constant.h"
#include "message/handler_base/http_base_message_handler.tpp"

std::string celeritas::email_bind_http_message_handler::get_supported_type_name() const
{
    return email_bind_path.data();
}

bool celeritas::email_bind_http_message_handler::handle(const http_handle_parameter_shared_ptr& handle_parameter,
                                                        const http_message_registry_weak_ptr& message_registry)
{
    co_spawn_response<email_bind>(handle_parameter, handler_channel, "email bind error:");

    return true;
}

std::string celeritas::email_bind_http_message_handler::get_server_type() const
{
    return auth_type.data();
}
