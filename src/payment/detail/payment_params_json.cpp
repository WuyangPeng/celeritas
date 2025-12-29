#include "payment_params_json.h"
#include "we_chat_payment_params_json.h"
#include "../../common/core_utilities/celeritas_error.h"

celeritas::payment_params_json::payment_params_json(const order_create_parameter& order_create_parameter)
    : order_create_parameter_{ order_create_parameter }
{
}

celeritas::payment_params_json::payment_params_json_unique_ptr celeritas::payment_params_json::create(const order_create_parameter& order_create_parameter)
{
    switch (const auto platform = order_create_parameter.get_platform())
    {
        case payment_platform_type::we_chat:
        {
            return std::make_unique<we_chat_payment_params_json>(order_create_parameter);
        }
        default:
        {
            throw celeritas_error{ "platform is error,platform = {}", static_cast<int>(platform) };
        }
    }
}