// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/database_data_type_traits.h"
#include "database/database_entity.h"
#include "database/database_entity_change.h"
#include "database/database_index_type.h"
#include "database/entity.h"

namespace celeritas
{
    class orders final : public database_entity
    {
    public:
        using class_type = orders;
        using base_type = database_entity;

        explicit orders(const database_entity_change& entity);

        orders(database_type database_type, const database_entity_change& entity);

        orders(database_type database_type, traits::param_type::int64_type id);

        [[nodiscard]] traits::int64_type get_id() const noexcept;

        [[nodiscard]] traits::string_type get_order_id() const;

        [[nodiscard]] traits::int64_type get_account_id() const noexcept;

        [[nodiscard]] traits::string_type get_game_server_id() const;

        [[nodiscard]] traits::int64_type get_role_id() const noexcept;

        [[nodiscard]] traits::string_type get_product_id() const;

        [[nodiscard]] traits::int32_type get_amount() const noexcept;

        [[nodiscard]] traits::int32_type get_platform() const noexcept;

        [[nodiscard]] traits::string_type get_transaction_id() const;

        [[nodiscard]] traits::int32_type get_status() const noexcept;

        [[nodiscard]] traits::int32_type get_delivery_status() const noexcept;

        [[nodiscard]] traits::string_type get_client_request_id() const;

        [[nodiscard]] traits::int32_type get_retry_count() const noexcept;

        [[nodiscard]] traits::int64_type get_create_time() const noexcept;

        [[nodiscard]] traits::int64_type get_pay_time() const noexcept;

        [[nodiscard]] traits::int64_type get_expire_time() const noexcept;

        [[nodiscard]] traits::string_type get_callback_data() const;

        [[nodiscard]] traits::int64_type get_refund_time() const noexcept;

        [[nodiscard]] traits::int32_type get_refund_amount() const noexcept;

        void set_id(traits::param_type::int64_type id);

        void set_order_id(traits::param_type::string_type order_id);

        void set_account_id(traits::param_type::int64_type account_id);

        void set_game_server_id(traits::param_type::string_type game_server_id);

        void set_role_id(traits::param_type::int64_type role_id);

        void set_product_id(traits::param_type::string_type product_id);

        void set_amount(traits::param_type::int32_type amount);

        void set_platform(traits::param_type::int32_type platform);

        void set_transaction_id(traits::param_type::string_type transaction_id);

        void set_status(traits::param_type::int32_type status);

        void set_delivery_status(traits::param_type::int32_type delivery_status);

        void set_client_request_id(traits::param_type::string_type client_request_id);

        void set_retry_count(traits::param_type::int32_type retry_count);

        void set_create_time(traits::param_type::int64_type create_time);

        void set_pay_time(traits::param_type::int64_type pay_time);

        void set_expire_time(traits::param_type::int64_type expire_time);

        void set_callback_data(traits::param_type::string_type callback_data);

        void set_refund_time(traits::param_type::int64_type refund_time);

        void set_refund_amount(traits::param_type::int32_type refund_amount);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static const database_field_container& get_mongo_database_field_container();

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, traits::param_type::int64_type id);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, const basis_database_container_const_shared_ptr& key);

        static constexpr std::string_view database_name{ "orders" };

        static constexpr std::string_view id_describe{ "id" };
        static constexpr std::string_view order_id_describe{ "order_id" };
        static constexpr std::string_view account_id_describe{ "account_id" };
        static constexpr std::string_view game_server_id_describe{ "game_server_id" };
        static constexpr std::string_view role_id_describe{ "role_id" };
        static constexpr std::string_view product_id_describe{ "product_id" };
        static constexpr std::string_view amount_describe{ "amount" };
        static constexpr std::string_view platform_describe{ "platform" };
        static constexpr std::string_view transaction_id_describe{ "transaction_id" };
        static constexpr std::string_view status_describe{ "status" };
        static constexpr std::string_view delivery_status_describe{ "delivery_status" };
        static constexpr std::string_view client_request_id_describe{ "client_request_id" };
        static constexpr std::string_view retry_count_describe{ "retry_count" };
        static constexpr std::string_view create_time_describe{ "create_time" };
        static constexpr std::string_view pay_time_describe{ "pay_time" };
        static constexpr std::string_view expire_time_describe{ "expire_time" };
        static constexpr std::string_view callback_data_describe{ "callback_data" };
        static constexpr std::string_view refund_time_describe{ "refund_time" };
        static constexpr std::string_view refund_amount_describe{ "refund_amount" };

    private:
        [[nodiscard]] static basis_database_container_const_shared_ptr get_key_basis_database_container(database_type database_type, traits::param_type::int64_type id);

        entity<id_describe, database_data_type::int64_type, database_index_type::key> id_;
        entity<order_id_describe, database_data_type::string_type, database_index_type::unique_index> order_id_;
        entity<account_id_describe, database_data_type::int64_type> account_id_;
        entity<game_server_id_describe, database_data_type::string_type> game_server_id_;
        entity<role_id_describe, database_data_type::int64_type, database_index_type::composite_index> role_id_;
        entity<product_id_describe, database_data_type::string_type> product_id_;
        entity<amount_describe, database_data_type::int32_type> amount_;
        entity<platform_describe, database_data_type::int32_type, database_index_type::composite_unique_index> platform_;
        entity<transaction_id_describe, database_data_type::string_type, database_index_type::composite_unique_index> transaction_id_;
        entity<status_describe, database_data_type::int32_type, database_index_type::composite_index> status_;
        entity<delivery_status_describe, database_data_type::int32_type> delivery_status_;
        entity<client_request_id_describe, database_data_type::string_type, database_index_type::unique_index> client_request_id_;
        entity<retry_count_describe, database_data_type::int32_type> retry_count_;
        entity<create_time_describe, database_data_type::int64_type> create_time_;
        entity<pay_time_describe, database_data_type::int64_type> pay_time_;
        entity<expire_time_describe, database_data_type::int64_type> expire_time_;
        entity<callback_data_describe, database_data_type::string_type> callback_data_;
        entity<refund_time_describe, database_data_type::int64_type> refund_time_;
        entity<refund_amount_describe, database_data_type::int32_type> refund_amount_;
    };
}