// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/basic/database_data_type_traits.h"
#include "database/basic/database_entity_change.h"
#include "database/basic/database_index_type.h"
#include "database/entity/database_entity.h"
#include "database/entity/entity.h"

namespace celeritas
{
    class user_red_dots final : public database_entity
    {
    public:
        using class_type = user_red_dots;
        using base_type = database_entity;

        explicit user_red_dots(const database_entity_change& entity);

        user_red_dots(database_type database_type, const database_entity_change& entity);

        user_red_dots(database_type database_type, traits::param_type::int64_type user_id);

        [[nodiscard]] traits::int64_type get_user_id() const noexcept;

        [[nodiscard]] traits::document_array_type get_red_dots() const;

        [[nodiscard]] traits::int64_type get_last_check_time() const noexcept;

        void set_user_id(traits::param_type::int64_type user_id);

        void set_red_dots(traits::param_type::document_array_type red_dots);

        void set_last_check_time(traits::param_type::int64_type last_check_time);

        void set_red_dots(int index, traits::param_type::document_array_element_type red_dots);

        void add_red_dots(traits::param_type::document_array_element_type red_dots);

        void remove_red_dots(int index);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, traits::param_type::int64_type user_id);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, const basis_database_container_const_shared_ptr& key);

        static constexpr std::string_view database_name{ "user_red_dots" };

        static constexpr std::string_view user_id_describe{ "_id" };
        static constexpr std::string_view red_dots_describe{ "red_dots" };
        static constexpr std::string_view last_check_time_describe{ "last_check_time" };

    private:
        [[nodiscard]] static basis_database_container_const_shared_ptr get_key_basis_database_container(traits::param_type::int64_type user_id);

        entity<user_id_describe, database_data_type::int64_type, database_index_type::key> user_id_;
        entity<red_dots_describe, database_data_type::document_array_type> red_dots_;
        entity<last_check_time_describe, database_data_type::int64_type> last_check_time_;
    };
}