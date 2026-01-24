#pragma once

#include "database/generated/mysql/payment/orders.h"
#include "detail/order_create_parameter.h"
#include "message/basic/http_service_base.h"

namespace celeritas
{
    class order_create final : public http_service_base
    {
    public:
        using class_type = order_create;
        using base_type = http_service_base;

        explicit order_create(http_handle_parameter_shared_ptr handle_parameter);

        [[nodiscard]] void_awaitable_type response() override;

        [[nodiscard]] void_awaitable_type send_error_response() override;

    private:
        using optional_orders = std::optional<orders>;
        using optional_orders_awaitable_type = boost::asio::awaitable<optional_orders>;
        using database_pool_shared_ptr = std::shared_ptr<database_pool_base>;
        using const_app_config_shared_ptr = std::shared_ptr<const app_config>;

        [[nodiscard]] static optional_orders_awaitable_type get_orders(const database_pool_shared_ptr& mysql_pool,
                                                                       const order_create_parameter& order_create_parameter,
                                                                       const const_app_config_shared_ptr& app_config);
    };
}
