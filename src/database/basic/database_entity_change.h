#pragma once

#include "basis_database.h"
#include "basis_database_container.h"
#include "config/config_fwd.h"

namespace celeritas
{
    // 调用非const函数时必须调用deep_copy_if_shared实现写时复制
    class database_entity_change
    {
    public:
        using class_type = database_entity_change;
        using const_basis_database_container_shared_ptr = std::shared_ptr<const basis_database_container>;

        database_entity_change(database_type database_type,
                               std::string_view database_name,
                               database_change_type change_type,
                               const_basis_database_container_shared_ptr key);

        database_entity_change(database_type database_type,
                               std::string_view database_name,
                               database_change_type change_type);

        [[nodiscard]] database_type get_database_type() const noexcept;

        [[nodiscard]] std::string_view get_database_name() const noexcept;

        [[nodiscard]] database_change_type get_change_type() const noexcept;

        [[nodiscard]] const_basis_database_container_shared_ptr get_key() const noexcept;

        [[nodiscard]] const_basis_database_container_shared_ptr get_database() const noexcept;

        [[nodiscard]] database_entity_change get_select() const;

        [[nodiscard]] database_entity_change get_select(const const_basis_database_container_shared_ptr& key) const;

        [[nodiscard]] database_entity_change get_database_entity_change(std::string_view field_name) const;

        void modify(const basis_database& basis_database);

        void clear();

        template <database_data_type Type>
        [[nodiscard]] database_data_type_traits<Type>::type get_value(std::string_view field_name,
                                                                      boost::call_traits<typename database_data_type_traits<Type>::type>::param_type default_value = typename database_data_type_traits<Type>::type{}) const;

        [[nodiscard]] bool is_modify() const;

        // 只有主键，其他值都是默认值时，数据库不保存
        [[nodiscard]] bool is_must_save() const;

    private:
        using basis_database_container_shared_ptr = std::shared_ptr<basis_database_container>;
        using value_variant = basis_database::value_variant;

        [[nodiscard]] const value_variant& get_variant_value(std::string_view field_name) const;

        template <database_data_type Type>
        [[nodiscard]] database_data_type_traits<Type>::type do_get_value(const value_variant& variant,
                                                                         boost::call_traits<typename database_data_type_traits<Type>::type>::param_type default_value) const;

        void deep_copy_if_shared();

        database_type database_type_;
        std::string_view database_name_;
        database_change_type change_type_;
        const_basis_database_container_shared_ptr key_;
        basis_database_container_shared_ptr database_;
    };
}