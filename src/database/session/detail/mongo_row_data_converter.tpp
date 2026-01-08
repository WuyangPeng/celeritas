#pragma once

#include "mongo_row_data_converter.h"

#include <bsoncxx/builder/basic/array.hpp>

template <celeritas::database_data_type T>
void celeritas::mongo_row_data_converter::append_basic_type(document_type& document, const basis_database& basis_database)
{
    document.append(bsoncxx::builder::basic::kvp(std::string{ basis_database.get_field_name() }, basis_database.get_value<T>()));
}

template <celeritas::database_data_type T>
void celeritas::mongo_row_data_converter::append_array_document(document_type& document, const basis_database& basis_database)
{
    bsoncxx::builder::basic::array basic{};
    for (const auto& element : basis_database.get_value<T>())
    {
        basic.append(element);
    }
    document.append(bsoncxx::builder::basic::kvp(std::string{ basis_database.get_field_name() }, basic));
}

template <typename T>
auto celeritas::mongo_row_data_converter::get_array_from_view(const array_type& row_view_array)
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
