// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/basic/database_data_type_traits.h"
#include "database/basic/database_entity_change.h"
#include "database/basic/database_index_type.h"
#include "database/entity/database_entity.h"
#include "database/entity/entity.h"

namespace celeritas
{
    class account_last_login final : public database_entity
    {
    public:
        using class_type = account_last_login;
        using base_type = database_entity;

        explicit account_last_login(const database_entity_change& entity);

        account_last_login(database_type database_type, const database_entity_change& entity);

        account_last_login(database_type database_type, traits::param_type::int64_type account_id);

        [[nodiscard]] traits::int64_type get_account_id() const noexcept;

        [[nodiscard]] traits::string_type get_game_server_id() const;

        [[nodiscard]] traits::int64_type get_update_time() const noexcept;

        void set_account_id(traits::param_type::int64_type account_id);

        void set_game_server_id(traits::param_type::string_type game_server_id);

        void set_update_time(traits::param_type::int64_type update_time);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, traits::param_type::int64_type account_id);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, const basis_database_container_const_shared_ptr& key);

        static constexpr std::string_view database_name{ "account_last_login" };

        static constexpr std::string_view account_id_describe{ "account_id" };
        static constexpr std::string_view game_server_id_describe{ "game_server_id" };
        static constexpr std::string_view update_time_describe{ "update_time" };

    private:
        [[nodiscard]] static basis_database_container_const_shared_ptr get_key_basis_database_container(traits::param_type::int64_type account_id);

        entity<account_id_describe, database_data_type::int64_type, database_index_type::key> account_id_;
        entity<game_server_id_describe, database_data_type::string_type> game_server_id_;
        entity<update_time_describe, database_data_type::int64_type> update_time_;
    };
}