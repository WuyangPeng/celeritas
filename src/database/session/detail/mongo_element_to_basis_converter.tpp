#pragma once

#include "mongo_element_to_basis_converter.h"

template <typename T>
auto celeritas::mongo_element_to_basis_converter::get_array_from_view(const array_type& row_view_array)
{
    std::vector<T> database_array{};
    for (const auto& element : row_view_array)
    {
        if constexpr (std::is_same_v<T, std::string>)
        {
            database_array.emplace_back(element.get_string().value);
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            database_array.emplace_back(element.get_double().value);
        }
        else if constexpr (std::is_same_v<T, int32_t>)
        {
            database_array.emplace_back(element.get_int32().value);
        }
        else if constexpr (std::is_same_v<T, int64_t>)
        {
            database_array.emplace_back(element.get_int64().value);
        }
    }
    return database_array;
}