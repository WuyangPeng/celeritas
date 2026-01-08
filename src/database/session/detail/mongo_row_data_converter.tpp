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
