#pragma once

#include "payment_parameter.h"
#include "database/generated/mysql/auth/apps.h"
#include "payment/order_create_http_response.h"
#include "payment/payment_platform_type.h"

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

        [[nodiscard]] std::string get_client_request_id() const;

        [[nodiscard]] int64_t get_account_id() const;

        [[nodiscard]] std::string get_game_server_id() const;

        [[nodiscard]] int64_t get_role_id() const;

        [[nodiscard]] std::string get_product_id() const;

        [[nodiscard]] int get_amount() const;

        [[nodiscard]] payment_platform_type get_platform() const;

    private:
        using optional_order_create_http_response = std::optional<order_create_http_response>;
        using optional_apps = std::optional<apps>;

        void init();

    private:
        optional_order_create_http_response response_;
        std::string client_request_id_;
        int64_t account_id_;
        std::string game_server_id_;
        int64_t role_id_;
        std::string product_id_;
        int amount_;
        payment_platform_type platform_;
        optional_apps apps_;
    };
}