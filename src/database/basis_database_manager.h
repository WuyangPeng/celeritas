#pragma once

#include "basis_database.h"
#include "basis_database_container.h"
#include "boost/call_traits.hpp"
#include "config/config_fwd.h"

#include <vector>

namespace celeritas
{
    class basis_database_manager
    {
    public:
        using class_type = basis_database_manager;

        using ObjectContainer = std::vector<basis_database>;

        basis_database_manager(database_type database_type,
                               std::string_view database_name,
                               database_change_type change_type,
                               basis_database_container key);

        basis_database_manager(database_type database_type,
                               std::string_view database_name,
                               database_change_type change_type,
                               basis_database_container key,
                               basis_database_container database);

        [[nodiscard]] database_type get_database_type() const noexcept;

        [[nodiscard]] std::string_view get_database_name() const noexcept;

        [[nodiscard]] database_change_type get_change_type() const noexcept;

        [[nodiscard]] basis_database_container get_key() const noexcept;

        [[nodiscard]] basis_database_container get_database() const noexcept;

        void modify(const basis_database& basis_database);

        void set(const basis_database_container& database);

        void clear();

        template <database_data_type Type>
        [[nodiscard]] database_data_Type_traits<Type>::Type get_value(std::string_view field_name, boost::call_traits<typename database_data_Type_traits<Type>::Type>::param_type default_value = typename database_data_Type_traits<Type>::Type{}) const;

        [[nodiscard]] bool is_modify() const;

    private:
        [[nodiscard]] std::any get_any_value(std::string_view field_name) const;

    private:
        database_type database_type_;
        std::string_view database_name_;
        database_change_type change_type_;
        basis_database_container key_;
        basis_database_container database_;
    };
}