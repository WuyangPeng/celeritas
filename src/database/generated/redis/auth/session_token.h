// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/database_data_type_traits.h"
#include "database/database_entity.h"
#include "database/database_entity_change.h"
#include "database/database_index_type.h"
#include "database/entity.h"

namespace celeritas
{
    class session_token final : public database_entity
    {
    public:
        using class_type = session_token;
        using base_type = database_entity;

        explicit session_token(const database_entity_change& entity);

        session_token(database_type database_type, traits::param_type::string_type token);

        [[nodiscard]] traits::string_type get_token() const;

        [[nodiscard]] traits::int64_type get_account_id() const noexcept;

        [[nodiscard]] traits::bool_type is_new_account() const noexcept;

        void set_token(traits::param_type::string_type token);

        void set_account_id(traits::param_type::int64_type account_id);

        void set_new_account(traits::param_type::bool_type new_account);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, traits::param_type::string_type token);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, const basis_database_container_const_shared_ptr& key);

        static constexpr std::string_view database_name{ "session_token" };

        static constexpr std::string_view token_describe{ "token" };
        static constexpr std::string_view account_id_describe{ "account_id" };
        static constexpr std::string_view new_account_describe{ "new_account" };

    private:
        [[nodiscard]] static basis_database_container_const_shared_ptr get_key_basis_database_container(database_type database_type, traits::param_type::string_type token);

        entity<token_describe, database_data_type::string_type, database_index_type::key> token_;
        entity<account_id_describe, database_data_type::int64_type> account_id_;
        entity<new_account_describe, database_data_type::bool_type> new_account_;
    };
}