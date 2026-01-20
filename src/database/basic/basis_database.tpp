#pragma once

#include "basis_database.h"

#include <boost/algorithm/string/replace.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <sstream>

template <typename T> requires (std::is_integral_v<T> && sizeof(T) <= sizeof(int32_t))
celeritas::basis_database::basis_database(const std::string_view field_name, T value)
    : class_type{ field_name, database_data_type::int32_type, boost::numeric_cast<int32_t>(value) }
{
}

template <typename T> requires (std::is_integral_v<T> && sizeof(T) == sizeof(int64_t))
celeritas::basis_database::basis_database(const std::string_view field_name, T value)
    : class_type{ field_name, database_data_type::int64_type, boost::numeric_cast<int64_t>(value) }
{
}

template <typename T> requires (std::is_floating_point_v<T>)
celeritas::basis_database::basis_database(const std::string_view field_name, T value)
    : class_type{ field_name, database_data_type::double_type, static_cast<double>(value) }
{
}

template <celeritas::database_data_type Type>
bool celeritas::basis_database::has_value() const
{
    using target_type = database_data_type_traits<Type>::type;
    return std::holds_alternative<target_type>(value_);
}

template <celeritas::database_data_type Type>
const celeritas::database_data_type_traits<Type>::type& celeritas::basis_database::get_value() const
{
    using target_type = database_data_type_traits<Type>::type;
    if (auto* value = std::get_if<target_type>(&value_))
    {
        return *value;
    }

    static const target_type default_value{};

    return default_value;
}

template <celeritas::database_data_type Type>
std::string celeritas::basis_database::get_array_string_value(typename database_data_type_traits<Type>::param_type value) const
{
    std::ostringstream os{};
    os << "[";
    for (auto iter = value.cbegin(); iter != value.cend();)
    {
        auto current = iter++;
        append_value<Type>(os, *current, iter == value.cend());
    }
    os << "]";

    return os.str();
}

template <celeritas::database_data_type Type>
void celeritas::basis_database::append_value(std::ostringstream& os, typename database_data_type_traits<Type>::element_param_type value, const bool is_last)
{
    if constexpr (Type == database_data_type::string_array_type)
    {
        auto result = value;
        boost::replace_all(result, "\"", "\\\"");
        os << "\"" + result + "\"";
    }
    else
    {
        os << value;
    }

    if (!is_last)
    {
        os << ",";
    }
}
