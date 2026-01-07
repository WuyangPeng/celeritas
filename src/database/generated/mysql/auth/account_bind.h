// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/basic/database_data_type_traits.h"
#include "database/basic/database_entity_change.h"
#include "database/basic/database_index_type.h"
#include "database/entity/database_entity.h"
#include "database/entity/entity.h"

namespace celeritas
{
    class account_bind final : public database_entity
    {
    public:
        using class_type = account_bind;
        using base_type = database_entity;

        explicit account_bind(const database_entity_change& entity);

        account_bind(database_type database_type, const database_entity_change& entity);

        account_bind(database_type database_type, traits::param_type::int64_type account_bind_id);

        [[nodiscard]] traits::int64_type get_account_bind_id() const noexcept;

        [[nodiscard]] traits::int64_type get_account_id() const noexcept;

        [[nodiscard]] traits::int32_type get_account_type() const noexcept;

        [[nodiscard]] traits::int32_type get_process_type() const noexcept;

        [[nodiscard]] traits::int64_type get_app_id() const noexcept;

        [[nodiscard]] traits::string_type get_auth_key() const;

        void set_account_bind_id(traits::param_type::int64_type account_bind_id);

        void set_account_id(traits::param_type::int64_type account_id);

        void set_account_type(traits::param_type::int32_type account_type);

        void set_process_type(traits::param_type::int32_type process_type);

        void set_app_id(traits::param_type::int64_type app_id);

        void set_auth_key(traits::param_type::string_type auth_key);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, traits::param_type::int64_type account_bind_id);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, const const_basis_database_container_shared_ptr& key);

        static constexpr std::string_view database_name{ "account_bind" };

        static constexpr std::string_view account_bind_id_describe{ "account_bind_id" };
        static constexpr std::string_view account_id_describe{ "account_id" };
        static constexpr std::string_view account_type_describe{ "account_type" };
        static constexpr std::string_view process_type_describe{ "process_type" };
        static constexpr std::string_view app_id_describe{ "app_id" };
        static constexpr std::string_view auth_key_describe{ "auth_key" };

    private:
        [[nodiscard]] static const_basis_database_container_shared_ptr get_key_basis_database_container(traits::param_type::int64_type account_bind_id);

        entity<account_bind_id_describe, database_data_type::int64_type, database_index_type::key> account_bind_id_;
        entity<account_id_describe, database_data_type::int64_type, database_index_type::index> account_id_;
        entity<account_type_describe, database_data_type::int32_type, database_index_type::composite_unique_index> account_type_;
        entity<process_type_describe, database_data_type::int32_type, database_index_type::composite_unique_index> process_type_;
        entity<app_id_describe, database_data_type::int64_type, database_index_type::composite_unique_index> app_id_;
        entity<auth_key_describe, database_data_type::string_type, database_index_type::composite_unique_index> auth_key_;
    };
}