// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/basic/database_data_type_traits.h"
#include "database/basic/database_index_type.h"
#include "database/entity/database_entity.h"
#include "database/entity/entity.h"

namespace celeritas
{
    class user_task final : public database_entity
    {
    public:
        using class_type = user_task;
        using base_type = database_entity;

        explicit user_task(const database_entity_change& entity);

        user_task(database_type database_type, const database_entity_change& entity);

        user_task(database_type database_type, traits::param_type::int64_type user_id);

        [[nodiscard]] traits::int64_type get_user_id() const noexcept;

        [[nodiscard]] traits::document_array_type get_task_data() const;

        [[nodiscard]] traits::int32_type get_data_version() const noexcept;

        [[nodiscard]] traits::int64_type get_last_save_time() const noexcept;

        void set_user_id(traits::param_type::int64_type user_id);

        void set_task_data(traits::param_type::document_array_type task_data);

        void set_data_version(traits::param_type::int32_type data_version);

        void set_last_save_time(traits::param_type::int64_type last_save_time);

        void set_task_data(int index, traits::param_type::document_array_element_type task_data);

        void add_task_data(traits::param_type::document_array_element_type task_data);

        void remove_task_data(int index);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, traits::param_type::int64_type user_id);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, const const_basis_database_container_shared_ptr& key);

        static constexpr std::string_view database_name{ "user_task" };

        static constexpr std::string_view user_id_describe{ "_id" };
        static constexpr std::string_view task_data_describe{ "task_data" };
        static constexpr std::string_view data_version_describe{ "data_version" };
        static constexpr std::string_view last_save_time_describe{ "last_save_time" };

    private:
        [[nodiscard]] static const_basis_database_container_shared_ptr get_key_basis_database_container(traits::param_type::int64_type user_id);

        entity<user_id_describe, database_data_type::int64_type, database_index_type::key> user_id_;
        entity<task_data_describe, database_data_type::document_array_type> task_data_;
        entity<data_version_describe, database_data_type::int32_type> data_version_;
        entity<last_save_time_describe, database_data_type::int64_type> last_save_time_;
    };
}