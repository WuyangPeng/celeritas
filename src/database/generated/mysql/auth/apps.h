// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/database_data_type_traits.h"
#include "database/database_entity.h"
#include "database/database_entity_change.h"
#include "database/database_index_type.h"
#include "database/entity.h"

namespace celeritas
{
    class apps final : public database_entity
    {
    public:
        using class_type = apps;
        using base_type = database_entity;

        explicit apps(const database_entity_change& entity);

        apps(database_type database_type, traits::param_type::int64_type app_id);

        [[nodiscard]] traits::int64_type get_app_id() const noexcept;

        [[nodiscard]] traits::string_type get_game_name() const;

        [[nodiscard]] traits::string_type get_app_secret() const;

        [[nodiscard]] traits::int32_type get_status() const noexcept;

        void set_app_id(traits::param_type::int64_type app_id);

        void set_game_name(traits::param_type::string_type game_name);

        void set_app_secret(traits::param_type::string_type app_secret);

        void set_status(traits::param_type::int32_type status);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, traits::param_type::int64_type app_id);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, const basis_database_container_const_shared_ptr& key);

        static constexpr std::string_view database_name{ "apps" };

        static constexpr std::string_view app_id_describe{ "app_id" };
        static constexpr std::string_view game_name_describe{ "game_name" };
        static constexpr std::string_view app_secret_describe{ "app_secret" };
        static constexpr std::string_view status_describe{ "status" };

    private:
        [[nodiscard]] static basis_database_container_const_shared_ptr get_key_basis_database_container(database_type database_type, traits::param_type::int64_type app_id);

        entity<app_id_describe, database_data_type::int64_type, database_index_type::key> app_id_;
        entity<game_name_describe, database_data_type::string_type, database_index_type::index> game_name_;
        entity<app_secret_describe, database_data_type::string_type> app_secret_;
        entity<status_describe, database_data_type::int32_type> status_;
    };
}