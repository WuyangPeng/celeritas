#include "we_chat_recharge_notify.h"

celeritas::we_chat_recharge_notify::we_chat_recharge_notify(sdk_payment_providers sdk_payment_providers, http_handle_parameter handle_parameter)
    : bass_type{ std::move(sdk_payment_providers), std::move(handle_parameter) }
{
}

celeritas::recharge_notify::void_awaitable_type celeritas::we_chat_recharge_notify::execute() const
{
    co_return;
}

std::string celeritas::we_chat_recharge_notify::get_default_message() const
{
    return "";
}