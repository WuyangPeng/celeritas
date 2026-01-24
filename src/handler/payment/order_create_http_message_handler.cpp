#include "order_create_http_message_handler.h"
#include "common/logging/logger.h"
#include "handler/handler_fwd.h"
#include "initializer/initializer_constant.h"
#include "payment/core/order_create.h"
#include "message/handler_base/http_base_message_handler.tpp"

std::string celeritas::order_create_http_message_handler::get_supported_type_name() const
{
    return order_create_path.data();
}

bool celeritas::order_create_http_message_handler::handle(const http_handle_parameter_shared_ptr& handle_parameter,
                                                          const http_message_registry_weak_ptr& message_registry)
{
    co_spawn_response<order_create>(handle_parameter, handler_channel, "order create error:");

    return true;
}

std::string celeritas::order_create_http_message_handler::get_server_type() const
{
    return payment_type.data();
}
