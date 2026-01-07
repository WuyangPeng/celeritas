// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/basic/database_data_type_traits.h"
#include "database/basic/database_index_type.h"
#include "database/entity/database_entity.h"
#include "database/entity/entity.h"

namespace celeritas
{
    class email_code final : public database_entity
    {
    public:
        using class_type = email_code;
        using base_type = database_entity;

        explicit email_code(const database_entity_change& entity);

        email_code(database_type database_type, const database_entity_change& entity);

        email_code(database_type database_type, traits::param_type::string_type email);

        [[nodiscard]] traits::string_type get_email() const;

        [[nodiscard]] traits::int32_type get_code() const noexcept;

        [[nodiscard]] traits::int32_count_type get_retry_count() const noexcept;

        void set_email(traits::param_type::string_type email);

        void set_code(traits::param_type::int32_type code);

        void set_retry_count(traits::param_type::int32_count_type retry_count);

        void modify_retry_count(traits::param_type::int32_count_type retry_count);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, traits::param_type::string_type email);

        [[nodiscard]] static const_database_entity_change_shared_ptr get_select(database_type database_type, const const_basis_database_container_shared_ptr& key);

        static constexpr std::string_view database_name{ "email_code" };

        static constexpr std::string_view email_describe{ "email" };
        static constexpr std::string_view code_describe{ "code" };
        static constexpr std::string_view retry_count_describe{ "retry_count" };

    private:
        [[nodiscard]] static const_basis_database_container_shared_ptr get_key_basis_database_container(traits::param_type::string_type email);

        entity<email_describe, database_data_type::string_type, database_index_type::key> email_;
        entity<code_describe, database_data_type::int32_type> code_;
        entity<retry_count_describe, database_data_type::int32_count_type> retry_count_;
    };
}