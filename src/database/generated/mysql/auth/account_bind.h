// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/database_data_type_traits.h"
#include "database/database_entity.h"
#include "database/database_index_type.h"
#include "database/entity.h"

namespace celeritas
{
    class account_bind final : public database_entity
    {
    public:
        using class_type = account_bind;
        using base_type = database_entity;

        [[nodiscard]] static account_bind create(const basis_database_manager& entity, database_type database_type, traits::param_type::int64_type account_bind_id);

        explicit account_bind(const basis_database_manager& entity);

        account_bind(database_type database_type, traits::param_type::int64_type account_bind_id);

        [[nodiscard]] traits::int64_type get_account_bind_id() const noexcept;

        [[nodiscard]] traits::int64_type get_account_id() const noexcept;

        [[nodiscard]] traits::int32_type get_account_type() const noexcept;

        [[nodiscard]] traits::string_type get_auth_key() const;

        [[nodiscard]] traits::bool_type is_is_primary() const noexcept;

        void set_account_bind_id(traits::param_type::int64_type account_bind_id);

        void set_account_id(traits::param_type::int64_type account_id);

        void set_account_type(traits::param_type::int32_type account_type);

        void set_auth_key(traits::param_type::string_type auth_key);

        void set_is_primary(traits::param_type::bool_type is_primary);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static basis_database_manager_const_hared_ptr get_select(database_type database_type, traits::param_type::int64_type account_bind_id);

        [[nodiscard]] static basis_database_manager_shared_ptr get_select(database_type database_type);

        static constexpr std::string_view database_name{ "account_bind" };

        static constexpr std::string_view account_bind_id_describe{ "account_bind_id" };
        static constexpr std::string_view account_id_describe{ "account_id" };
        static constexpr std::string_view account_type_describe{ "account_type" };
        static constexpr std::string_view auth_key_describe{ "auth_key" };
        static constexpr std::string_view is_primary_describe{ "is_primary" };

    private:
        [[nodiscard]] static basis_database_container get_key_basis_database_container(database_type database_type, traits::param_type::int64_type account_bind_id);

        entity<account_bind_id_describe, database_data_type::int64_type, database_index_type::key> account_bind_id_;
        entity<account_id_describe, database_data_type::int64_type, database_index_type::index> account_id_;
        entity<account_type_describe, database_data_type::int32_type, database_index_type::composite_unique_index> account_type_;
        entity<auth_key_describe, database_data_type::string_type, database_index_type::composite_unique_index> auth_key_;
        entity<is_primary_describe, database_data_type::bool_type> is_primary_;
    };
}