#pragma once

#include "mysql_row_data_converter.h"
#include "database/basic/database_field.h"

#include <boost/lexical_cast.hpp>

#include <ranges>

template <typename ArrayType>
celeritas::basis_database celeritas::mysql_row_data_converter::to_numeric_array_basis(const database_field& field_name, const field_view_type& row_view)
{
    std::string value{ row_view.as_string() };

    if (value.starts_with("[") && value.ends_with("]"))
    {
        value = value.substr(1, value.length() - 2);
    }

    if (value.empty())
    {
        return basis_database{ field_name.get_field_name(), ArrayType{} };
    }

    auto split_view = value | std::views::split(',');

    auto int_view = split_view | std::views::transform([](const auto& subrange) {
        const std::string result{ subrange.begin(), subrange.end() };
        return boost::lexical_cast<typename ArrayType::value_type>(result);
    });
    const ArrayType result{ int_view.begin(), int_view.end() };

    return basis_database{ field_name.get_field_name(), result };
}
