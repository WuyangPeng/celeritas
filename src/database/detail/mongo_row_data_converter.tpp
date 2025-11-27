#pragma once

#include "mongo_row_data_converter.h"

#include <boost/lexical_cast.hpp>

#include <ranges>

template <typename ArrayType>
celeritas::basis_database celeritas::mongo_row_data_converter::to_numeric_array_basis(const database_field& field, const document_element_type& row_view)
{
    const std::string value{ row_view.get_string().value };
    if (value.empty())
    {
        return basis_database{ field.get_field_name(), ArrayType{} };
    }

    auto split_view = value | std::views::split('|');

    auto transformed_view = split_view | std::views::transform(
                                [](const auto& subrange) {
                                    const std::string result_str{ subrange.begin(), subrange.end() };
                                    return boost::lexical_cast<typename ArrayType::value_type>(result_str);
                                });

    const ArrayType result{ transformed_view.begin(), transformed_view.end() };
    return basis_database{ field.get_field_name(), result };
}