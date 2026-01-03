#include "notify_http_message_handler.h"
#include "common/logging/logger.h"
#include "handler/handler_fwd.h"
#include "initializer/initializer_constant.h"
#include "initializer/initializer_fwd.h"
#include "message/http_handle_parameter.h"
#include "payment/recharge_notify.h"

celeritas::notify_http_message_handler::notify_http_message_handler(const sdk_payment_providers_key& sdk_payment_providers_key, std::string path_suffix)
    : sdk_payment_providers_key_{ sdk_payment_providers_key }, path_suffix_{ std::move(path_suffix) }
{
}

std::string celeritas::notify_http_message_handler::get_supported_type_name() const
{
    return notify_path.data() + path_suffix_;
}

bool celeritas::notify_http_message_handler::handle(const http_handle_parameter_shared_ptr& handle_parameter, const http_message_registry_weak_ptr& message_registry)
{
    if (handle_parameter->get_server_type() != payment_type)
    {
        return false;
    }

    boost::asio::co_spawn(handle_parameter->get_io_context(),
                          response(sdk_payment_providers_key_, handle_parameter),
                          boost::asio::detached);

    return true;
}

celeritas::notify_http_message_handler::void_awaitable_type celeritas::notify_http_message_handler::response(const sdk_payment_providers_key sdk_payment_providers_key, http_handle_parameter_shared_ptr handle_parameter)
{
    const auto recharge_notify = recharge_notify::create(sdk_payment_providers_key, handle_parameter);

    try
    {
        co_await recharge_notify->execute();
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(handler_channel, error) << "notify error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(handler_channel, fatal) << "notify unknown error.";
    }

    co_return co_await handle_parameter->write_immediately(recharge_notify->get_default_message());
}