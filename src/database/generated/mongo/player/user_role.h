// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/basic/database_data_type_traits.h"
#include "database/basic/database_entity_change.h"
#include "database/basic/database_index_type.h"
#include "database/entity/database_entity.h"
#include "database/entity/entity.h"

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

        [[nodiscard]] traits::string_type get_surname() const;

        [[nodiscard]] traits::string_type get_name() const;

        [[nodiscard]] traits::bool_type is_modify_name() const noexcept;

        [[nodiscard]] traits::int32_type get_change_count() const noexcept;

        [[nodiscard]] traits::int32_type get_per_day_change_count() const noexcept;

        [[nodiscard]] traits::string_type get_device_id() const;

        [[nodiscard]] traits::string_type get_app_version() const;

        [[nodiscard]] traits::int64_type get_change_name_time() const noexcept;

        [[nodiscard]] traits::string_type get_full_name() const;

        void set_user_id(traits::param_type::int64_type user_id);

        void set_surname(traits::param_type::string_type surname);

        void set_name(traits::param_type::string_type name);

        void set_modify_name(traits::param_type::bool_type modify_name);

        void set_change_count(traits::param_type::int32_type change_count);

        void set_per_day_change_count(traits::param_type::int32_type per_day_change_count);

        void set_device_id(traits::param_type::string_type device_id);

        void set_app_version(traits::param_type::string_type app_version);

        void set_change_name_time(traits::param_type::int64_type change_name_time);

        void set_full_name(traits::param_type::string_type full_name);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, traits::param_type::int64_type user_id);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, const const_basis_database_container_shared_ptr& key);

        static constexpr std::string_view database_name{ "user_role" };

        static constexpr std::string_view user_id_describe{ "_id" };
        static constexpr std::string_view surname_describe{ "surname" };
        static constexpr std::string_view name_describe{ "name" };
        static constexpr std::string_view modify_name_describe{ "modify_name" };
        static constexpr std::string_view change_count_describe{ "change_count" };
        static constexpr std::string_view per_day_change_count_describe{ "per_day_change_count" };
        static constexpr std::string_view device_id_describe{ "device_id" };
        static constexpr std::string_view app_version_describe{ "app_version" };
        static constexpr std::string_view change_name_time_describe{ "change_name_time" };
        static constexpr std::string_view full_name_describe{ "full_name" };

    private:
        [[nodiscard]] static const_basis_database_container_shared_ptr get_key_basis_database_container(traits::param_type::int64_type user_id);

        entity<user_id_describe, database_data_type::int64_type, database_index_type::key> user_id_;
        entity<surname_describe, database_data_type::string_type> surname_;
        entity<name_describe, database_data_type::string_type> name_;
        entity<modify_name_describe, database_data_type::bool_type> modify_name_;
        entity<change_count_describe, database_data_type::int32_type> change_count_;
        entity<per_day_change_count_describe, database_data_type::int32_type> per_day_change_count_;
        entity<device_id_describe, database_data_type::string_type> device_id_;
        entity<app_version_describe, database_data_type::string_type> app_version_;
        entity<change_name_time_describe, database_data_type::int64_type> change_name_time_;
        entity<full_name_describe, database_data_type::string_type, database_index_type::unique_index> full_name_;
    };
}