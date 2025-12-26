// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/database_data_type_traits.h"
#include "database/database_entity.h"
#include "database/database_entity_change.h"
#include "database/database_index_type.h"
#include "database/entity.h"

namespace celeritas
{
    class character_session final : public database_entity
    {
    public:
        using class_type = character_session;
        using base_type = database_entity;

        explicit character_session(const database_entity_change& entity);

        character_session(database_type database_type, const database_entity_change& entity);

        character_session(database_type database_type, traits::param_type::int64_type account_id);

        [[nodiscard]] traits::int64_type get_account_id() const noexcept;

        [[nodiscard]] traits::string_type get_player_server_instance_id() const;

        void set_account_id(traits::param_type::int64_type account_id);

        void set_player_server_instance_id(traits::param_type::string_type player_server_instance_id);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static const database_field_container& get_mongo_database_field_container();

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, traits::param_type::int64_type account_id);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, const basis_database_container_const_shared_ptr& key);

        static constexpr std::string_view database_name{ "character_session" };

        static constexpr std::string_view account_id_describe{ "account_id" };
        static constexpr std::string_view player_server_instance_id_describe{ "player_server_instance_id" };

    private:
        [[nodiscard]] static basis_database_container_const_shared_ptr get_key_basis_database_container(database_type database_type, traits::param_type::int64_type account_id);

        entity<account_id_describe, database_data_type::int64_type, database_index_type::key> account_id_;
        entity<player_server_instance_id_describe, database_data_type::string_type> player_server_instance_id_;
    };
}