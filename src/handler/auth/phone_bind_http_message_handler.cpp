#include "phone_bind_http_message_handler.h"
#include "auth/phone/phone_bind.h"
#include "common/logging/logger.h"
#include "handler/handler_fwd.h"
#include "initializer/initializer_constant.h"

std::string celeritas::phone_bind_http_message_handler::get_supported_type_name() const
{
    return phone_bind_path.data();
}

bool celeritas::phone_bind_http_message_handler::handle(const http_handle_parameter_shared_ptr& handle_parameter,
                                                        const http_message_registry_weak_ptr& message_registry)
{
    co_spawn_response<phone_bind>(handle_parameter, handler_channel, "phone bind error:");

    return true;
}

std::string celeritas::phone_bind_http_message_handler::get_server_type() const
{
    return auth_type.data();
}
