// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/database_data_type_traits.h"
#include "database/database_entity.h"
#include "database/database_entity_change.h"
#include "database/database_index_type.h"
#include "database/entity.h"

namespace celeritas
{
    class user_time_refresh final : public database_entity
    {
    public:
        using class_type = user_time_refresh;
        using base_type = database_entity;

        explicit user_time_refresh(const database_entity_change& entity);

        user_time_refresh(database_type database_type, const database_entity_change& entity);

        user_time_refresh(database_type database_type, traits::param_type::int64_type user_id);

        [[nodiscard]] traits::int64_type get_user_id() const noexcept;

        [[nodiscard]] traits::document_array_type get_player_time_refresh() const;

        void set_user_id(traits::param_type::int64_type user_id);

        void set_player_time_refresh(traits::param_type::document_array_type player_time_refresh);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, traits::param_type::int64_type user_id);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, const basis_database_container_const_shared_ptr& key);

        static constexpr std::string_view database_name{ "user_time_refresh" };

        static constexpr std::string_view user_id_describe{ "user_id" };
        static constexpr std::string_view player_time_refresh_describe{ "player_time_refresh" };

    private:
        [[nodiscard]] static basis_database_container_const_shared_ptr get_key_basis_database_container(database_type database_type, traits::param_type::int64_type user_id);

        entity<user_id_describe, database_data_type::int64_type, database_index_type::key> user_id_;
        entity<player_time_refresh_describe, database_data_type::document_array_type> player_time_refresh_;
    };
}