// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/database_data_type_traits.h"
#include "database/database_entity.h"
#include "database/database_entity_change.h"
#include "database/database_index_type.h"
#include "database/entity.h"

namespace celeritas
{
    class time_refresh final : public database_entity
    {
    public:
        using class_type = time_refresh;
        using base_type = database_entity;

        explicit time_refresh(const database_entity_change& entity);

        time_refresh(database_type database_type, const database_entity_change& entity);

        time_refresh(database_type database_type, traits::param_type::int64_type id);

        [[nodiscard]] traits::int64_type get_id() const noexcept;

        [[nodiscard]] traits::int32_type get_time_refresh_type() const noexcept;

        [[nodiscard]] traits::int32_type get_parameter() const noexcept;

        void set_id(traits::param_type::int64_type id);

        void set_time_refresh_type(traits::param_type::int32_type time_refresh_type);

        void set_parameter(traits::param_type::int32_type parameter);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static const database_field_container& get_mongo_database_field_container();

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, traits::param_type::int64_type id);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, const basis_database_container_const_shared_ptr& key);

        static constexpr std::string_view database_name{ "time_refresh" };

        static constexpr std::string_view id_describe{ "id" };
        static constexpr std::string_view time_refresh_type_describe{ "time_refresh_type" };
        static constexpr std::string_view parameter_describe{ "parameter" };

    private:
        [[nodiscard]] static basis_database_container_const_shared_ptr get_key_basis_database_container(database_type database_type, traits::param_type::int64_type id);

        entity<id_describe, database_data_type::int64_type, database_index_type::key> id_;
        entity<time_refresh_type_describe, database_data_type::int32_type> time_refresh_type_;
        entity<parameter_describe, database_data_type::int32_type> parameter_;
    };
}