#pragma once

#include "basis_to_mongo_appender.h"
#include "database/basic/basis_database.tpp"

#include <bsoncxx/builder/basic/array.hpp>

template <celeritas::database_data_type T>
void celeritas::basis_to_mongo_appender::append_basic_type(document_type& document, const basis_database& basis_database)
{
    document.append(bsoncxx::builder::basic::kvp(std::string{ basis_database.get_field_name() }, basis_database.get_value<T>()));
}

template <celeritas::database_data_type T>
void celeritas::basis_to_mongo_appender::append_array_document(document_type& document, const basis_database& basis_database)
{
    bsoncxx::builder::basic::array basic{};
    for (const auto& element : basis_database.get_value<T>())
    {
        basic.append(element);
    }
    document.append(bsoncxx::builder::basic::kvp(std::string{ basis_database.get_field_name() }, basic));
}
