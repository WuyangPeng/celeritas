// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/database_data_type_traits.h"
#include "database/database_entity.h"
#include "database/database_entity_change.h"
#include "database/database_index_type.h"
#include "database/entity.h"

namespace celeritas
{
    class user_role final : public database_entity
    {
    public:
        using class_type = user_role;
        using base_type = database_entity;

        explicit user_role(const database_entity_change& entity);

        user_role(database_type database_type, const database_entity_change& entity);

        user_role(database_type database_type, traits::param_type::int64_type user_id);

        [[nodiscard]] traits::int64_type get_user_id() const noexcept;

        [[nodiscard]] traits::string_type get_name() const;

        [[nodiscard]] traits::string_type get_device_id() const;

        [[nodiscard]] traits::string_type get_app_version() const;

        void set_user_id(traits::param_type::int64_type user_id);

        void set_name(traits::param_type::string_type name);

        void set_device_id(traits::param_type::string_type device_id);

        void set_app_version(traits::param_type::string_type app_version);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static const database_field_container& get_mongo_database_field_container();

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, traits::param_type::int64_type user_id);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, const basis_database_container_const_shared_ptr& key);

        static constexpr std::string_view database_name{ "user_role" };

        static constexpr std::string_view user_id_describe{ "user_id" };
        static constexpr std::string_view name_describe{ "name" };
        static constexpr std::string_view device_id_describe{ "device_id" };
        static constexpr std::string_view app_version_describe{ "app_version" };

    private:
        [[nodiscard]] static basis_database_container_const_shared_ptr get_key_basis_database_container(database_type database_type, traits::param_type::int64_type user_id);

        entity<user_id_describe, database_data_type::int64_type, database_index_type::key> user_id_;
        entity<name_describe, database_data_type::string_type> name_;
        entity<device_id_describe, database_data_type::string_type> device_id_;
        entity<app_version_describe, database_data_type::string_type> app_version_;
    };
}