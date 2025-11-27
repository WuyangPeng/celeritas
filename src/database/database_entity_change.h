#pragma once

#include "basis_database.h"
#include "basis_database_container.h"
#include "boost/call_traits.hpp"
#include "config/config_fwd.h"

namespace celeritas
{
    class database_entity_change
    {
    public:
        using class_type = database_entity_change;
        using basis_database_container_const_shared_ptr = std::shared_ptr<const basis_database_container>;

        database_entity_change(database_type database_type,
                               std::string_view database_name,
                               database_change_type change_type,
                               basis_database_container_const_shared_ptr key);

        database_entity_change(database_type database_type,
                               std::string_view database_name,
                               database_change_type change_type);

        [[nodiscard]] database_type get_database_type() const noexcept;

        [[nodiscard]] std::string_view get_database_name() const noexcept;

        [[nodiscard]] database_change_type get_change_type() const noexcept;

        [[nodiscard]] basis_database_container_const_shared_ptr get_key() const noexcept;

        [[nodiscard]] basis_database_container_const_shared_ptr get_database() const noexcept;

        [[nodiscard]] database_entity_change get_select() const;

        [[nodiscard]] database_entity_change get_select(const basis_database_container_const_shared_ptr& key) const;

        void modify(const basis_database& basis_database);

        void clear();

        template <database_data_type Type>
        [[nodiscard]] database_data_Type_traits<Type>::Type get_value(std::string_view field_name, boost::call_traits<typename database_data_Type_traits<Type>::Type>::param_type default_value = typename database_data_Type_traits<Type>::Type{}) const;

        [[nodiscard]] bool is_modify() const;

    private:
        using basis_database_container_shared_ptr = std::shared_ptr<basis_database_container>;

        [[nodiscard]] std::any get_any_value(std::string_view field_name) const;

        database_type database_type_;
        std::string_view database_name_;
        database_change_type change_type_;
        basis_database_container_const_shared_ptr key_;
        basis_database_container_shared_ptr database_;
    };
}