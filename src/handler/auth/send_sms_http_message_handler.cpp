#include "send_sms_http_message_handler.h"
#include "auth/phone/send_sms.h"
#include "common/logging/logger.h"
#include "handler/handler_constant.h"
#include "initializer/initializer_constant.h"
#include "message/handler_base/http_base_message_handler.tpp"

std::string celeritas::send_sms_http_message_handler::get_supported_type_name() const
{
    return send_sms_path.data();
}

bool celeritas::send_sms_http_message_handler::handle(const http_handle_parameter_shared_ptr& handle_parameter,
                                                      const http_message_registry_weak_ptr& message_registry)
{
    co_spawn_response<send_sms>(handle_parameter, handler_channel, "send sms error:");

    return true;
}

std::string celeritas::send_sms_http_message_handler::get_server_type() const
{
    return auth_type.data();
}

