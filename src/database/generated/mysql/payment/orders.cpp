// 此文件是自动生成，请勿手动修改。

#include "orders.h"
#include "config/basic/database_type.h"
#include "database/basic/database_change_type.h"
#include "database/basic/database_entity_change.tpp"
#include "database/entity/database_entity.tpp"
#include "database/entity/entity.tpp"

celeritas::orders::orders(const database_entity_change& entity)
    : base_type{ entity },
      id_{ entity.get_value<database_data_type::int64_type>(id_describe) },
      order_id_{ entity.get_value<database_data_type::string_type>(order_id_describe) },
      account_id_{ entity.get_value<database_data_type::int64_type>(account_id_describe) },
      game_server_id_{ entity.get_value<database_data_type::string_type>(game_server_id_describe) },
      role_id_{ entity.get_value<database_data_type::int64_type>(role_id_describe) },
      product_id_{ entity.get_value<database_data_type::string_type>(product_id_describe) },
      amount_{ entity.get_value<database_data_type::int32_type>(amount_describe) },
      platform_{ entity.get_value<database_data_type::int32_type>(platform_describe) },
      transaction_id_{ entity.get_value<database_data_type::string_type>(transaction_id_describe) },
      status_{ entity.get_value<database_data_type::int32_type>(status_describe) },
      delivery_status_{ entity.get_value<database_data_type::int32_type>(delivery_status_describe) },
      client_request_id_{ entity.get_value<database_data_type::string_type>(client_request_id_describe) },
      retry_count_{ entity.get_value<database_data_type::int32_type>(retry_count_describe) },
      create_time_{ entity.get_value<database_data_type::int64_type>(create_time_describe) },
      pay_time_{ entity.get_value<database_data_type::int64_type>(pay_time_describe) },
      expire_time_{ entity.get_value<database_data_type::int64_type>(expire_time_describe) },
      callback_data_{ entity.get_value<database_data_type::string_type>(callback_data_describe) },
      refund_time_{ entity.get_value<database_data_type::int64_type>(refund_time_describe) },
      refund_amount_{ entity.get_value<database_data_type::int32_type>(refund_amount_describe) }
{
}

celeritas::orders::orders(const database_type database_type, const database_entity_change& entity)
    : base_type{ database_type, entity },
      id_{ entity.get_value<database_data_type::int64_type>(id_describe) },
      order_id_{ entity.get_value<database_data_type::string_type>(order_id_describe) },
      account_id_{ entity.get_value<database_data_type::int64_type>(account_id_describe) },
      game_server_id_{ entity.get_value<database_data_type::string_type>(game_server_id_describe) },
      role_id_{ entity.get_value<database_data_type::int64_type>(role_id_describe) },
      product_id_{ entity.get_value<database_data_type::string_type>(product_id_describe) },
      amount_{ entity.get_value<database_data_type::int32_type>(amount_describe) },
      platform_{ entity.get_value<database_data_type::int32_type>(platform_describe) },
      transaction_id_{ entity.get_value<database_data_type::string_type>(transaction_id_describe) },
      status_{ entity.get_value<database_data_type::int32_type>(status_describe) },
      delivery_status_{ entity.get_value<database_data_type::int32_type>(delivery_status_describe) },
      client_request_id_{ entity.get_value<database_data_type::string_type>(client_request_id_describe) },
      retry_count_{ entity.get_value<database_data_type::int32_type>(retry_count_describe) },
      create_time_{ entity.get_value<database_data_type::int64_type>(create_time_describe) },
      pay_time_{ entity.get_value<database_data_type::int64_type>(pay_time_describe) },
      expire_time_{ entity.get_value<database_data_type::int64_type>(expire_time_describe) },
      callback_data_{ entity.get_value<database_data_type::string_type>(callback_data_describe) },
      refund_time_{ entity.get_value<database_data_type::int64_type>(refund_time_describe) },
      refund_amount_{ entity.get_value<database_data_type::int32_type>(refund_amount_describe) }
{
    if (database_type != entity.get_database_type())
    {
        add_modify(id_describe, get_id());
        add_modify(order_id_describe, get_order_id());
        add_modify(account_id_describe, get_account_id());
        add_modify(game_server_id_describe, get_game_server_id());
        add_modify(role_id_describe, get_role_id());
        add_modify(product_id_describe, get_product_id());
        add_modify(amount_describe, get_amount());
        add_modify(platform_describe, get_platform());
        add_modify(transaction_id_describe, get_transaction_id());
        add_modify(status_describe, get_status());
        add_modify(delivery_status_describe, get_delivery_status());
        add_modify(client_request_id_describe, get_client_request_id());
        add_modify(retry_count_describe, get_retry_count());
        add_modify(create_time_describe, get_create_time());
        add_modify(pay_time_describe, get_pay_time());
        add_modify(expire_time_describe, get_expire_time());
        add_modify(callback_data_describe, get_callback_data());
        add_modify(refund_time_describe, get_refund_time());
        add_modify(refund_amount_describe, get_refund_amount());
    }
}

celeritas::orders::orders(const database_type database_type, traits::param_type::int64_type id)
    : base_type{ database_type, database_name, get_key_basis_database_container(id) },
      id_{ id },
      order_id_{ traits::string_type{} },
      account_id_{ traits::int64_type{} },
      game_server_id_{ traits::string_type{} },
      role_id_{ traits::int64_type{} },
      product_id_{ traits::string_type{} },
      amount_{ traits::int32_type{} },
      platform_{ traits::int32_type{} },
      transaction_id_{ traits::string_type{} },
      status_{ traits::int32_type{} },
      delivery_status_{ traits::int32_type{} },
      client_request_id_{ traits::string_type{} },
      retry_count_{ traits::int32_type{} },
      create_time_{ traits::int64_type{} },
      pay_time_{ traits::int64_type{} },
      expire_time_{ traits::int64_type{} },
      callback_data_{ traits::string_type{} },
      refund_time_{ traits::int64_type{} },
      refund_amount_{ traits::int32_type{} }
{
    add_modify(id_describe, id);
}

celeritas::traits::int64_type celeritas::orders::get_id() const noexcept
{
    return id_.get_value();
}

celeritas::traits::string_type celeritas::orders::get_order_id() const
{
    return order_id_.get_value();
}

celeritas::traits::int64_type celeritas::orders::get_account_id() const noexcept
{
    return account_id_.get_value();
}

celeritas::traits::string_type celeritas::orders::get_game_server_id() const
{
    return game_server_id_.get_value();
}

celeritas::traits::int64_type celeritas::orders::get_role_id() const noexcept
{
    return role_id_.get_value();
}

celeritas::traits::string_type celeritas::orders::get_product_id() const
{
    return product_id_.get_value();
}

celeritas::traits::int32_type celeritas::orders::get_amount() const noexcept
{
    return amount_.get_value();
}

celeritas::traits::int32_type celeritas::orders::get_platform() const noexcept
{
    return platform_.get_value();
}

celeritas::traits::string_type celeritas::orders::get_transaction_id() const
{
    return transaction_id_.get_value();
}

celeritas::traits::int32_type celeritas::orders::get_status() const noexcept
{
    return status_.get_value();
}

celeritas::traits::int32_type celeritas::orders::get_delivery_status() const noexcept
{
    return delivery_status_.get_value();
}

celeritas::traits::string_type celeritas::orders::get_client_request_id() const
{
    return client_request_id_.get_value();
}

celeritas::traits::int32_type celeritas::orders::get_retry_count() const noexcept
{
    return retry_count_.get_value();
}

celeritas::traits::int64_type celeritas::orders::get_create_time() const noexcept
{
    return create_time_.get_value();
}

celeritas::traits::int64_type celeritas::orders::get_pay_time() const noexcept
{
    return pay_time_.get_value();
}

celeritas::traits::int64_type celeritas::orders::get_expire_time() const noexcept
{
    return expire_time_.get_value();
}

celeritas::traits::string_type celeritas::orders::get_callback_data() const
{
    return callback_data_.get_value();
}

celeritas::traits::int64_type celeritas::orders::get_refund_time() const noexcept
{
    return refund_time_.get_value();
}

celeritas::traits::int32_type celeritas::orders::get_refund_amount() const noexcept
{
    return refund_amount_.get_value();
}

void celeritas::orders::set_id(traits::param_type::int64_type id)
{
    if (id != get_id())
    {
        id_.set_value(id);

        add_modify(id_describe, get_id());
    }
}

void celeritas::orders::set_order_id(traits::param_type::string_type order_id)
{
    if (order_id != get_order_id())
    {
        order_id_.set_value(order_id);

        add_modify(order_id_describe, get_order_id());
    }
}

void celeritas::orders::set_account_id(traits::param_type::int64_type account_id)
{
    if (account_id != get_account_id())
    {
        account_id_.set_value(account_id);

        add_modify(account_id_describe, get_account_id());
    }
}

void celeritas::orders::set_game_server_id(traits::param_type::string_type game_server_id)
{
    if (game_server_id != get_game_server_id())
    {
        game_server_id_.set_value(game_server_id);

        add_modify(game_server_id_describe, get_game_server_id());
    }
}

void celeritas::orders::set_role_id(traits::param_type::int64_type role_id)
{
    if (role_id != get_role_id())
    {
        role_id_.set_value(role_id);

        add_modify(role_id_describe, get_role_id());
    }
}

void celeritas::orders::set_product_id(traits::param_type::string_type product_id)
{
    if (product_id != get_product_id())
    {
        product_id_.set_value(product_id);

        add_modify(product_id_describe, get_product_id());
    }
}

void celeritas::orders::set_amount(traits::param_type::int32_type amount)
{
    if (amount != get_amount())
    {
        amount_.set_value(amount);

        add_modify(amount_describe, get_amount());
    }
}

void celeritas::orders::set_platform(traits::param_type::int32_type platform)
{
    if (platform != get_platform())
    {
        platform_.set_value(platform);

        add_modify(platform_describe, get_platform());
    }
}

void celeritas::orders::set_transaction_id(traits::param_type::string_type transaction_id)
{
    if (transaction_id != get_transaction_id())
    {
        transaction_id_.set_value(transaction_id);

        add_modify(transaction_id_describe, get_transaction_id());
    }
}

void celeritas::orders::set_status(traits::param_type::int32_type status)
{
    if (status != get_status())
    {
        status_.set_value(status);

        add_modify(status_describe, get_status());
    }
}

void celeritas::orders::set_delivery_status(traits::param_type::int32_type delivery_status)
{
    if (delivery_status != get_delivery_status())
    {
        delivery_status_.set_value(delivery_status);

        add_modify(delivery_status_describe, get_delivery_status());
    }
}

void celeritas::orders::set_client_request_id(traits::param_type::string_type client_request_id)
{
    if (client_request_id != get_client_request_id())
    {
        client_request_id_.set_value(client_request_id);

        add_modify(client_request_id_describe, get_client_request_id());
    }
}

void celeritas::orders::set_retry_count(traits::param_type::int32_type retry_count)
{
    if (retry_count != get_retry_count())
    {
        retry_count_.set_value(retry_count);

        add_modify(retry_count_describe, get_retry_count());
    }
}

void celeritas::orders::set_create_time(traits::param_type::int64_type create_time)
{
    if (create_time != get_create_time())
    {
        create_time_.set_value(create_time);

        add_modify(create_time_describe, get_create_time());
    }
}

void celeritas::orders::set_pay_time(traits::param_type::int64_type pay_time)
{
    if (pay_time != get_pay_time())
    {
        pay_time_.set_value(pay_time);

        add_modify(pay_time_describe, get_pay_time());
    }
}

void celeritas::orders::set_expire_time(traits::param_type::int64_type expire_time)
{
    if (expire_time != get_expire_time())
    {
        expire_time_.set_value(expire_time);

        add_modify(expire_time_describe, get_expire_time());
    }
}

void celeritas::orders::set_callback_data(traits::param_type::string_type callback_data)
{
    if (callback_data != get_callback_data())
    {
        callback_data_.set_value(callback_data);

        add_modify(callback_data_describe, get_callback_data());
    }
}

void celeritas::orders::set_refund_time(traits::param_type::int64_type refund_time)
{
    if (refund_time != get_refund_time())
    {
        refund_time_.set_value(refund_time);

        add_modify(refund_time_describe, get_refund_time());
    }
}

void celeritas::orders::set_refund_amount(traits::param_type::int32_type refund_amount)
{
    if (refund_amount != get_refund_amount())
    {
        refund_amount_.set_value(refund_amount);

        add_modify(refund_amount_describe, get_refund_amount());
    }
}

const celeritas::database_entity::database_field_container& celeritas::orders::get_database_field_container()
{
    static const database_field_container field_name_container{ decltype(id_)::get_database_field(),
                                                                decltype(order_id_)::get_database_field(),
                                                                decltype(account_id_)::get_database_field(),
                                                                decltype(game_server_id_)::get_database_field(),
                                                                decltype(role_id_)::get_database_field(),
                                                                decltype(product_id_)::get_database_field(),
                                                                decltype(amount_)::get_database_field(),
                                                                decltype(platform_)::get_database_field(),
                                                                decltype(transaction_id_)::get_database_field(),
                                                                decltype(status_)::get_database_field(),
                                                                decltype(delivery_status_)::get_database_field(),
                                                                decltype(client_request_id_)::get_database_field(),
                                                                decltype(retry_count_)::get_database_field(),
                                                                decltype(create_time_)::get_database_field(),
                                                                decltype(pay_time_)::get_database_field(),
                                                                decltype(expire_time_)::get_database_field(),
                                                                decltype(callback_data_)::get_database_field(),
                                                                decltype(refund_time_)::get_database_field(),
                                                                decltype(refund_amount_)::get_database_field() };

    return field_name_container;
}

celeritas::orders::database_entity_change_const_shared_ptr celeritas::orders::get_select(const database_type database_type)
{
    static const auto result = std::make_shared<database_entity_change>(database_type,
                                                                        database_name,
                                                                        database_change_type::select_type);

    return result;
}

celeritas::orders::database_entity_change_const_shared_ptr celeritas::orders::get_select(const database_type database_type, traits::param_type::int64_type id)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    get_key_basis_database_container(id));
}

celeritas::orders::database_entity_change_const_shared_ptr celeritas::orders::get_select(const database_type database_type, const basis_database_container_const_shared_ptr& key)
{
    return std::make_shared<database_entity_change>(database_type,
                                                    database_name,
                                                    database_change_type::select_type,
                                                    key);
}

celeritas::orders::basis_database_container_const_shared_ptr celeritas::orders::get_key_basis_database_container(traits::param_type::int64_type id)
{
    return std::make_shared<basis_database_container>(basis_database{ id_describe, id });
}