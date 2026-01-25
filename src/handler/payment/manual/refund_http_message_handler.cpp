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
    if (handle_parameter->get_server_type() != payment_type)
    {
        return false;
    }

    boost::asio::co_spawn(handle_parameter->get_any_io_executor(),
                          response(sdk_payment_providers_key_, handle_parameter),
                          boost::asio::detached);

    return true;
}

celeritas::refund_http_message_handler::void_awaitable_type celeritas::refund_http_message_handler::response(const sdk_payment_providers_key sdk_payment_providers_key, http_handle_parameter_shared_ptr handle_parameter)
{
    try
    {
        const auto refund_notify = refund_notify::create(sdk_payment_providers_key, std::move(handle_parameter));

        co_await refund_notify->execute();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(handler_channel, error) << "refund error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(handler_channel, fatal) << "refund unknown error.";
    }

    co_return;
}