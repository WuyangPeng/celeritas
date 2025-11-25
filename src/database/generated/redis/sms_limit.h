// 此文件是自动生成，请勿手动修改。

#pragma once

#include "database/database_entity.h"
#include "database/database_data_type_traits.h"
#include "database/database_index_type.h"
#include "database/entity.h"

namespace celeritas
{
    class sms_limit final : public database_entity
    {
    public:
        using class_type = sms_limit;
        using base_type = database_entity;

        [[nodiscard]] static sms_limit create(const basis_database_manager& entity, database_type database_type, traits::param_type::string_type phone);

        explicit sms_limit(const basis_database_manager& entity);

        sms_limit(database_type database_type, traits::param_type::string_type phone);

        [[nodiscard]] traits::string_type get_phone() const;

        [[nodiscard]] traits::bool_type is_exist() const noexcept;

        void set_phone(traits::param_type::string_type phone);

        void set_exist(traits::param_type::bool_type exist);

        [[nodiscard]] static const database_field_container& get_database_field_container();

        [[nodiscard]] static basis_database_manager_const_hared_ptr get_select(database_type database_type, traits::param_type::string_type phone);

        [[nodiscard]] static basis_database_manager_shared_ptr get_select(database_type database_type);

        [[nodiscard]] static constexpr std::string_view get_database_name()
        {
            return database_name;
        }

        static constexpr std::string_view phone_describe{ "phone" };
        static constexpr std::string_view exist_describe{ "exist" };

    private:
        static constexpr std::string_view database_name{ "sms_limit" };

        [[nodiscard]] static basis_database_container get_key_basis_database_container(database_type database_type, traits::param_type::string_type phone);

        entity<phone_describe, database_data_type::string_type, database_index_type::key> phone_;
        entity<exist_describe, database_data_type::bool_type> exist_;
    };
}