// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/database_data_type_traits.h"
#include "database/database_entity.h"
#include "database/database_entity_change.h"
#include "database/database_index_type.h"
#include "database/entity.h"

namespace celeritas
{
    class sms_code final : public database_entity
    {
    public:
        using class_type = sms_code;
        using base_type = database_entity;

        [[nodiscard]] static sms_code create(const database_entity_change& entity, database_type database_type, traits::param_type::string_type phone);

        explicit sms_code(const database_entity_change& entity);

        sms_code(database_type database_type, traits::param_type::string_type phone);

        [[nodiscard]] traits::string_type get_phone() const;

        [[nodiscard]] traits::int32_type get_code() const noexcept;

        [[nodiscard]] traits::int32_count_type get_retry_count() const noexcept;

        void set_phone(traits::param_type::string_type phone);

        void set_code(traits::param_type::int32_type code);

        void set_retry_count(traits::param_type::int32_count_type retry_count);

        void modify_retry_count(traits::param_type::int32_count_type retry_count);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, traits::param_type::string_type phone);

        [[nodiscard]] static database_entity_change_const_shared_ptr get_select(database_type database_type, const basis_database_container_const_shared_ptr& key);

        static constexpr std::string_view database_name{ "sms_code" };

        static constexpr std::string_view phone_describe{ "phone" };
        static constexpr std::string_view code_describe{ "code" };
        static constexpr std::string_view retry_count_describe{ "retry_count" };

    private:
        [[nodiscard]] static basis_database_container_const_shared_ptr get_key_basis_database_container(database_type database_type, traits::param_type::string_type phone);

        entity<phone_describe, database_data_type::string_type, database_index_type::key> phone_;
        entity<code_describe, database_data_type::int32_type> code_;
        entity<retry_count_describe, database_data_type::int32_count_type> retry_count_;
    };
}