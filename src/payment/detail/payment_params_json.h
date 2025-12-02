#pragma once

#include "order_create_parameter.h"

namespace celeritas
{
    class payment_params_json
    {
    public:
        using class_type = payment_params_json;
        using payment_params_json_unique_ptr = std::unique_ptr<class_type>;

        explicit payment_params_json(const order_create_parameter& order_create_parameter);

        virtual ~payment_params_json() noexcept = default;

        payment_params_json(const payment_params_json& rhs) noexcept = default;

        payment_params_json& operator=(const payment_params_json& rhs) noexcept = delete;

        payment_params_json(payment_params_json&& rhs) noexcept = default;

        payment_params_json& operator=(payment_params_json&& rhs) noexcept = delete;

        [[nodiscard]] virtual std::string get_payment_params_json() const = 0;

        [[nodiscard]] static payment_params_json_unique_ptr create(const order_create_parameter& order_create_parameter);

    private:
        const order_create_parameter& order_create_parameter_;
    };
}