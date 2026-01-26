#include "refund_http_message_handler.h"
#include "common/logging/logger.h"
#include "initializer/initializer_constant.h"
#include "message/parameters/http_handle_parameter.h"
#include "payment/notification/refund_notify.h"
#include "handler/handler_constant.h"

celeritas::refund_http_message_handler::refund_http_message_handler(const sdk_payment_providers_key& sdk_payment_providers_key, std::string path_suffix)
    : sdk_payment_providers_key_{ sdk_payment_providers_key }, path_suffix_{ std::move(path_suffix) }
{
}

std::string celeritas::refund_http_message_handler::get_supported_type_name() const
{
    return refund_path.data() + path_suffix_;
}

bool celeritas::refund_http_message_handler::handle(const http_handle_parameter_shared_ptr& handle_parameter, const http_message_registry_weak_ptr& message_registry)
{
    co_spawn(handle_parameter->get_any_io_executor(),
             noexcept_safe_call_and_log_awaitable([handle_parameter = handle_parameter,
                                                      sdk_payment_providers_key = sdk_payment_providers_key_] {
                                                      return response(sdk_payment_providers_key, handle_parameter);
                                                  },
                                                  handler_channel,
                                                  "refund error: "),

             boost::asio::detached);

    return true;
}

std::string celeritas::refund_http_message_handler::get_server_type() const
{
    return payment_type.data();
}

celeritas::refund_http_message_handler::void_awaitable_type celeritas::refund_http_message_handler::response(const sdk_payment_providers_key sdk_payment_providers_key, http_handle_parameter_shared_ptr handle_parameter)
{
    const auto refund_notify = refund_notify::create(sdk_payment_providers_key, std::move(handle_parameter));

    co_await refund_notify->execute();
}