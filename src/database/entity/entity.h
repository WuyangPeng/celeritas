#pragma once

#include "database/database_fwd.h"
#include "database/basic/database_index_type.h"

#include <boost/call_traits.hpp>

#include <string_view>
#include <type_traits>

namespace celeritas
{
    template <const std::string_view& FieldName, database_data_type Type, database_index_type Index = database_index_type::null>
    class entity
    {
    public:
        using class_type = entity;
        using entity_type = database_data_type_traits<Type>::type;
        using element_type = database_data_type_traits<Type>::element_type;

        explicit entity(boost::call_traits<entity_type>::param_type entity) noexcept(std::is_nothrow_copy_constructible_v<entity_type>);

        [[nodiscard]] static constexpr std::string_view get_field_name() noexcept
        {
            return FieldName;
        }

        [[nodiscard]] static constexpr database_data_type get_data_type() noexcept
        {
            return Type;
        }

        [[nodiscard]] static constexpr database_index_type get_index_type() noexcept
        {
            return Index;
        }

        [[nodiscard]] static database_field get_database_field() noexcept;

        [[nodiscard]] entity_type get_value() const noexcept(std::is_nothrow_copy_constructible_v<entity_type>);

        void set_value(boost::call_traits<entity_type>::param_type entity) noexcept(std::is_nothrow_copy_assignable_v<entity_type>);

        void modify_value(boost::call_traits<entity_type>::param_type entity) noexcept(noexcept(std::declval<entity_type&>() += std::declval<typename boost::call_traits<entity_type>::param_type>()));

        [[nodiscard]] bool set_value(int index, boost::call_traits<element_type>::param_type entity);

        void add_value(boost::call_traits<element_type>::param_type entity);

        void remove_value(int index);

    private:
        entity_type entity_;
    };
}
