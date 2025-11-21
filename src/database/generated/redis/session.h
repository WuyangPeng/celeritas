// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/database_entity.h"
#include "database/database_data_type_traits.h"
#include "database/database_index_type.h"
#include "database/entity.h"

namespace celeritas
{
    class session final : public database_entity
    {
    public:
        using class_type = session;
        using base_type = database_entity;

        [[nodiscard]] static session create(const basis_database_manager& entity, database_type database_type, traits::param_type::string_type token);

        explicit session(const basis_database_manager& entity);

        session(database_type database_type, traits::param_type::string_type token);

        [[nodiscard]] traits::string_type get_token() const;

        [[nodiscard]] traits::int64_type get_account_id() const noexcept;

        void set_token(traits::param_type::string_type token);

        void set_account_id(traits::param_type::int64_type account_id);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static basis_database_manager_const_hared_ptr get_select(database_type database_type, traits::param_type::string_type token);

        [[nodiscard]] static basis_database_manager_shared_ptr get_select_all(database_type database_type);

        [[nodiscard]] static constexpr std::string_view get_database_name()
        {
            return database_name;
        }

        static constexpr std::string_view token_describe{ "_id" };
        static constexpr std::string_view account_id_describe{ "account_id" };

    private:
        static constexpr std::string_view database_name{ "session" };

        [[nodiscard]] static basis_database_container get_key_basis_database_container(traits::param_type::string_type token);

        entity<token_describe, database_data_type::string_type, database_index_type::key> token_;
        entity<account_id_describe, database_data_type::int64_type> account_id_;
    };
}