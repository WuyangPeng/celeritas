#pragma once

#include "json_value_to_basis_converter.h"

template <typename Container, typename UnaryOperation>
celeritas::basis_database celeritas::json_value_to_basis_converter::convert_array_impl(std::string_view key, const boost::json::array& array, UnaryOperation op)
{
    Container result{};
    result.reserve(array.size());
    for (const auto& element : array)
    {
        result.emplace_back(op(element));
    }
    return basis_database{ key, result };
}