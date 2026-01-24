#pragma once

#include "payment_params_json.h"

namespace celeritas
{
    class we_chat_payment_params_json final : public payment_params_json
    {
    public:
        using class_type = we_chat_payment_params_json;
        using base_type = payment_params_json;

        explicit we_chat_payment_params_json(const order_create_parameter& order_create_parameter);

        [[nodiscard]] std::string get_payment_params_json() const override;
    };
}