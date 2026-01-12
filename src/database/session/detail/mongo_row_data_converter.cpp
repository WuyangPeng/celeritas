#include "basis_to_mongo_appender.h"
#include "mongo_element_to_basis_converter.h"
#include "mongo_row_data_converter.h"
#include "mongo_to_basis_converter.tpp"
#include "common/core/celeritas_error.h"
#include "database/basic/basis_database_container.h"
#include "database/basic/database_field.h"

#include <boost/algorithm/string/classification.hpp>
#include <bsoncxx/builder/basic/array.hpp>

#include <functional>
#include <map>

using namespace std::literals;

celeritas::basis_database celeritas::mongo_row_data_converter::get_basis_database(const database_field_container& field_name_container,
                                                                                  const document_element_type& row_view)
{
    const auto iter = std::ranges::find_if(field_name_container,
                                           [key = row_view.key()](const auto& value) {
                                               return key == value.get_field_name();
                                           });

    if (iter == field_name_container.cend())
    {
        throw celeritas_error{ "field name is error,name ={}", row_view.key() };
    }

    const auto& container = mongo_to_basis_converter::get_basis_from_field();

    if (const auto database = container.find(iter->get_data_type());
        database != container.cend())
    {
        return database->second(row_view);
    }

    return basis_database{ iter->get_field_name(), std::string{} };
}

celeritas::mongo_row_data_converter::document_shared_ptr celeritas::mongo_row_data_converter::get_document(const const_basis_database_container_shared_ptr& container)
{
    basis_to_mongo_appender appender{};

    for (const auto& value : *container)
    {
        appender.append_document(value);
    }

    return appender.get_document();
}

celeritas::basis_database celeritas::mongo_row_data_converter::get_basis_database(const document_element_type& row_view)
{
    const auto& container = mongo_element_to_basis_converter::get_element_converters();

    if (const auto iter = container.find(row_view.type());
        iter != container.cend())
    {
        return iter->second(row_view);
    }

    throw celeritas_error{ "Unsupported type in mongo row data." };
}
