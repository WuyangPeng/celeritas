#pragma once

#include "mongo_to_basis_converter.h"

#include <bsoncxx/types.hpp>

template <typename T>
std::vector<T> celeritas::mongo_to_basis_converter::get_numeric_array(const array_type& array_view)
{
    std::vector<T> result{};
    for (const auto& element : array_view)
    {
        if constexpr (std::is_same_v<T, int32_t>)
        {
            result.emplace_back(element.get_int32().value);
        }
        else if constexpr (std::is_same_v<T, int64_t>)
        {
            if (element.type() == bsoncxx::type::k_int32)
            {
                result.emplace_back(element.get_int32().value);
            }
            else
            {
                result.emplace_back(element.get_int64().value);
            }
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            result.emplace_back(element.get_double().value);
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            result.emplace_back(element.get_string().value);
        }
    }

    return result;
}