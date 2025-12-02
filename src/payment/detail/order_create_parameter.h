#pragma once

#include "payment_parameter.h"
#include "database/generated/mysql/auth/apps.h"
#include "payment/order_create_http_response.h"

namespace celeritas
{
    class order_create_parameter final : public payment_parameter
    {
    public:
        using class_type = order_create_parameter;
        using base_type = payment_parameter;

        explicit order_create_parameter(const http_handle_parameter& http_handle_parameter);

        [[nodiscard]] bool is_failure() const override;

        [[nodiscard]] order_create_http_response get_response() const;

        [[nodiscard]] apps get_apps() const;

    private:
        using optional_order_create_http_response = std::optional<order_create_http_response>;
        using optional_apps = std::optional<apps>;

        void init();

        optional_order_create_http_response response_;
        std::string client_request_id_;
        int64_t account_id_;
        std::string game_server_id_;
        int64_t role_id_;
        std::string product_id_;
        int amount_;
        std::string platform_;
        optional_apps apps_;
    };
}