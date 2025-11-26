#pragma once

#include "basis_database.h"
#include "database_data_type.h"
#include "database_data_type_traits.h"

#include <sstream>

template <typename T> requires (std::is_integral_v<T>)
celeritas::basis_database::basis_database(const std::string_view field_name, T value)
    : class_type{ field_name, database_data_type::int32_type, value }
{
}

template <typename T> requires (std::is_floating_point_v<T>)
celeritas::basis_database::basis_database(const std::string_view field_name, T value)
    : class_type{ field_name, database_data_type::double_type, value }
{
}

template <celeritas::database_data_type Type>
celeritas::database_data_Type_traits<Type>::Type celeritas::basis_database::get_value() const
{
    if (const auto value = get_any_value();
        value.has_value())
    {
        return std::any_cast<typename database_data_Type_traits<Type>::Type>(value);
    }

    return typename database_data_Type_traits<Type>::Type{};
}

template <celeritas::database_data_type Type>
std::string celeritas::basis_database::get_array_string_value() const
{
    const auto value = get_value<Type>();
    std::ostringstream os{};
    for (auto iter = value.cbegin(); iter != value.cend(); ++iter)
    {
        os << *iter;
        if (std::next(iter) != value.cend())
        {
            os << "|";
        }
    }
    return os.str();
}
