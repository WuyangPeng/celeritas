#include "we_chat_refund_notify.h"

celeritas::we_chat_refund_notify::we_chat_refund_notify(sdk_payment_providers sdk_payment_providers, http_handle_parameter_shared_ptr handle_parameter)
    : bass_type{ std::move(sdk_payment_providers), std::move(handle_parameter) }
{
}

celeritas::refund_notify::void_awaitable_type celeritas::we_chat_refund_notify::execute() const
{
    co_return;
}