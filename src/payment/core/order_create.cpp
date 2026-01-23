#include "order_create.h"
#include "payment/basic/payment_delivery_status_type.h"
#include "payment/payment_fwd.h"
#include "payment/basic/payment_status_type.h"
#include "common/core/snowflake_generator.h"
#include "common/core/time_helper.h"
#include "config/aggregate/app_config.h"
#include "database/database_constant.h"
#include "database/pool/database_pool_manager.h"
#include "database/generated/mysql/payment/orders.h"
#include "payment/detail/payment_params_json.h"

celeritas::order_create::order_create(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::order_create::void_awaitable_type celeritas::order_create::response()
{
    const order_create_parameter order_create_parameter{ get_http_handle_parameter() };
    if (order_create_parameter.is_failure())
    {
        co_return co_await write_immediately(order_create_parameter.get_response());
    }

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(payment_db_name.data());

    const auto optional_order = co_await get_orders(mysql_pool, order_create_parameter, get_app_config());
    if (!optional_order)
    {
        co_return co_await write_immediately(order_create_http_response{ game_error_type::mysql_error });
    }

    const auto& order = *optional_order;

    co_return co_await write_immediately(order_create_http_response{ game_error_type::success,
                                                                     "order create success.",
                                                                     order.get_order_id(),
                                                                     static_cast<payment_platform_type>(order.get_platform()),
                                                                     payment_params_json::create(order_create_parameter)->get_payment_params_json(),
                                                                     order.get_amount() });
}

celeritas::http_service_base::void_awaitable_type celeritas::order_create::send_error_response()
{
    co_return co_await write_immediately(order_create_http_response{ game_error_type::unknown });
}

celeritas::order_create::optional_orders_awaitable_type celeritas::order_create::get_orders(const database_pool_shared_ptr& mysql_pool,
                                                                                            const order_create_parameter& order_create_parameter,
                                                                                            const const_app_config_shared_ptr& app_config)
{
    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { orders::client_request_id_describe, order_create_parameter.get_client_request_id() } });

    if (const auto optional_order = co_await mysql_pool->select_one(orders::get_select(database_type::mysql, key), orders::get_database_field_container()))
    {
        co_return orders{ *optional_order };
    }

    const auto server_config = app_config->get_server_config();
    const auto id = snowflake_generator::get_instance().generate(server_config->get_datacenter_id(), server_config->get_worker_id());

    orders orders{ database_type::mysql, id };
    orders.set_order_id(generate_token());
    orders.set_account_id(order_create_parameter.get_account_id());
    orders.set_game_server_id(order_create_parameter.get_game_server_id());
    orders.set_role_id(order_create_parameter.get_role_id());
    orders.set_product_id(order_create_parameter.get_product_id());
    orders.set_amount(order_create_parameter.get_amount());
    orders.set_platform(static_cast<int>(order_create_parameter.get_platform()));
    orders.set_transaction_id(orders.get_order_id());
    orders.set_status(static_cast<int>(payment_status_type::pending));
    orders.set_delivery_status(static_cast<int>(payment_delivery_status_type::waiting_delivery));
    orders.set_client_request_id(order_create_parameter.get_client_request_id());
    orders.set_create_time(time_helper::get_current_milliseconds());
    orders.set_expire_time(orders.get_create_time() + default_order_timeout);

    if (co_await mysql_pool->execute_changes(orders.get_modify()))
    {
        co_return orders;
    }

    co_return std::nullopt;
}



