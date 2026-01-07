// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/basic/database_data_type_traits.h"
#include "database/basic/database_entity_change.h"
#include "database/basic/database_index_type.h"
#include "database/entity/database_entity.h"
#include "database/entity/entity.h"

namespace celeritas
{
    class user final : public database_entity
    {
    public:
        using class_type = user;
        using base_type = database_entity;

        explicit user(const database_entity_change& entity);

        user(database_type database_type, const database_entity_change& entity);

        user(database_type database_type, traits::param_type::int64_type user_id);

        [[nodiscard]] traits::int64_type get_user_id() const noexcept;

        [[nodiscard]] traits::int64_type get_account_id() const noexcept;

        [[nodiscard]] traits::string_type get_game_server_id() const;

        [[nodiscard]] traits::bool_type is_overload_db() const noexcept;

        void set_user_id(traits::param_type::int64_type user_id);

        void set_account_id(traits::param_type::int64_type account_id);

        void set_game_server_id(traits::param_type::string_type game_server_id);

        void set_overload_db(traits::param_type::bool_type overload_db);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, traits::param_type::int64_type user_id);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, const const_basis_database_container_shared_ptr& key);

        static constexpr std::string_view database_name{ "user" };

        static constexpr std::string_view user_id_describe{ "user_id" };
        static constexpr std::string_view account_id_describe{ "account_id" };
        static constexpr std::string_view game_server_id_describe{ "game_server_id" };
        static constexpr std::string_view overload_db_describe{ "overload_db" };

    private:
        [[nodiscard]] static const_basis_database_container_shared_ptr get_key_basis_database_container(traits::param_type::int64_type user_id);

        entity<user_id_describe, database_data_type::int64_type, database_index_type::key> user_id_;
        entity<account_id_describe, database_data_type::int64_type, database_index_type::composite_unique_key_and_index> account_id_;
        entity<game_server_id_describe, database_data_type::string_type, database_index_type::composite_unique_index> game_server_id_;
        entity<overload_db_describe, database_data_type::bool_type> overload_db_;
    };
}