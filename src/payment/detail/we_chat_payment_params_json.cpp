#include "we_chat_payment_params_json.h"

celeritas::we_chat_payment_params_json::we_chat_payment_params_json(const order_create_parameter& order_create_parameter)
    : base_type{ order_create_parameter }
{
}

std::string celeritas::we_chat_payment_params_json::get_payment_params_json() const
{
    return "";
}