// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/database_data_type_traits.h"
#include "database/database_entity.h"
#include "database/database_entity_change.h"
#include "database/database_index_type.h"
#include "database/entity.h"

namespace celeritas
{
    class user_server_roles final : public database_entity
    {
    public:
        using class_type = user_server_roles;
        using base_type = database_entity;

        explicit user_server_roles(const database_entity_change& entity);

        user_server_roles(database_type database_type, const database_entity_change& entity);

        user_server_roles(database_type database_type, traits::param_type::int64_type id);

        [[nodiscard]] traits::int64_type get_id() const noexcept;

        [[nodiscard]] traits::document_array_type get_servers() const;

        [[nodiscard]] traits::int64_type get_update_time() const noexcept;

        void set_id(traits::param_type::int64_type id);

        void set_servers(traits::param_type::document_array_type servers);

        void set_update_time(traits::param_type::int64_type update_time);

        void set_servers(int index, traits::param_type::document_array_element_type servers);

        void add_servers(traits::param_type::document_array_element_type servers);

        void remove_servers(int index);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, traits::param_type::int64_type id);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, const basis_database_container_const_shared_ptr& key);

        static constexpr std::string_view database_name{ "user_server_roles" };

        static constexpr std::string_view id_describe{ "id" };
        static constexpr std::string_view servers_describe{ "servers" };
        static constexpr std::string_view update_time_describe{ "update_time" };

    private:
        [[nodiscard]] static basis_database_container_const_shared_ptr get_key_basis_database_container(database_type database_type, traits::param_type::int64_type id);

        entity<id_describe, database_data_type::int64_type, database_index_type::key> id_;
        entity<servers_describe, database_data_type::document_array_type> servers_;
        entity<update_time_describe, database_data_type::int64_type> update_time_;
    };
}