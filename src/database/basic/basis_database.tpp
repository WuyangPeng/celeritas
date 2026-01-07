#pragma once

#include "basis_database.h"

#include <sstream>

template <typename T> requires (std::is_integral_v<T>)
celeritas::basis_database::basis_database(const std::string_view field_name, T value)
    : class_type{ field_name, database_data_type::int32_type, static_cast<int32_t>(value) }
{
}

template <typename T> requires (std::is_floating_point_v<T>)
celeritas::basis_database::basis_database(const std::string_view field_name, T value)
    : class_type{ field_name, database_data_type::double_type, static_cast<double>(value) }
{
}

template <celeritas::database_data_type Type>
celeritas::database_data_type_traits<Type>::type celeritas::basis_database::get_value() const
{
    if (const auto value = get_any_value();
        value.has_value())
    {
        return std::any_cast<typename database_data_type_traits<Type>::type>(value);
    }

    return typename database_data_type_traits<Type>::type{};
}

template <celeritas::database_data_type Type>
std::string celeritas::basis_database::get_array_string_value() const
{
    const auto value = get_value<Type>();
    std::ostringstream os{};
    os << "[";
    for (auto iter = value.cbegin(); iter != value.cend(); ++iter)
    {
        if constexpr (Type == database_data_type::string_array_type)
        {
            os << "\"" << *iter << "\"";
        }
        else
        {
            os << *iter;
        }

        if (std::next(iter) != value.cend())
        {
            os << ",";
        }
    }
    os << "]";
    return os.str();
}
