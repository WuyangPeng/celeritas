// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/database_entity.h"
#include "database/database_data_type_traits.h"
#include "database/database_index_type.h"
#include "database/entity.h"

namespace celeritas
{
    class sms_code final : public database_entity
    {
    public:
        using class_type = sms_code;
        using base_type = database_entity;

        [[nodiscard]] static sms_code create(const basis_database_manager& entity, database_type database_type, traits::param_type::string_type phone);

        explicit sms_code(const basis_database_manager& entity);

        sms_code(database_type database_type, traits::param_type::string_type phone);

        [[nodiscard]] traits::string_type get_phone() const;

        [[nodiscard]] traits::int32_type get_code() const noexcept;

        void set_phone(traits::param_type::string_type phone);

        void set_code(traits::param_type::int32_type code);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static basis_database_manager_const_hared_ptr get_select(database_type database_type, traits::param_type::string_type phone);

        [[nodiscard]] static basis_database_manager_shared_ptr get_select_all(database_type database_type);

        [[nodiscard]] static constexpr std::string_view get_database_name()
        {
            return database_name;
        }

        static constexpr std::string_view phone_describe{ "phone" };
        static constexpr std::string_view code_describe{ "code" };

    private:
        static constexpr std::string_view database_name{ "sms_code" };

        [[nodiscard]] static basis_database_container get_key_basis_database_container(database_type database_type, traits::param_type::string_type phone);

        entity<phone_describe, database_data_type::string_type, database_index_type::key> phone_;
        entity<code_describe, database_data_type::int32_type> code_;
    };
}