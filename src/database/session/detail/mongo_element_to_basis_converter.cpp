#include "mongo_element_to_basis_converter.h"
#include "mongo_row_data_converter.h"
#include "mongo_array_to_basis_converter.h"
#include "common/core/celeritas_error.h"

const celeritas::mongo_element_to_basis_converter::container_type& celeritas::mongo_element_to_basis_converter::get_element_converters()
{
    static const container_type container{ { bsoncxx::type::k_double, get_double_from_element },
                                           { bsoncxx::type::k_string, get_string_from_element },
                                           { bsoncxx::type::k_bool, get_bool_from_element },
                                           { bsoncxx::type::k_document, get_document_from_element },
                                           { bsoncxx::type::k_int32, get_int32_from_element },
                                           { bsoncxx::type::k_int64, get_int64_from_element },
                                           { bsoncxx::type::k_array, get_array_basis_database_from_view } };

    return container;
}

celeritas::basis_database celeritas::mongo_element_to_basis_converter::get_double_from_element(const document_element_type& row_view)
{
    return { row_view.key(), row_view.get_double().value };
}

celeritas::basis_database celeritas::mongo_element_to_basis_converter::get_string_from_element(const document_element_type& row_view)
{
    return { row_view.key(), std::string{ row_view.get_string().value } };
}

celeritas::basis_database celeritas::mongo_element_to_basis_converter::get_bool_from_element(const document_element_type& row_view)
{
    return { row_view.key(), row_view.get_bool().value };
}

celeritas::basis_database celeritas::mongo_element_to_basis_converter::get_document_from_element(const document_element_type& row_view)
{
    basis_database::document_type document{};
    for (const auto& element : row_view.get_document().value)
    {
        document.emplace_back(mongo_row_data_converter::get_basis_database(element));
    }
    return { row_view.key(), document };
}

celeritas::basis_database celeritas::mongo_element_to_basis_converter::get_int32_from_element(const document_element_type& row_view)
{
    return { row_view.key(), row_view.get_int32().value };
}

celeritas::basis_database celeritas::mongo_element_to_basis_converter::get_int64_from_element(const document_element_type& row_view)
{
    return { row_view.key(), row_view.get_int64().value };
}

celeritas::basis_database celeritas::mongo_element_to_basis_converter::get_array_basis_database_from_view(const document_element_type& row_view)
{
    const auto row_view_array = row_view.get_array().value;

    if (row_view_array.empty())
    {
        return { row_view.key(), basis_database::int32_array{} };
    }

    const auto& converters = mongo_array_to_basis_converter::get_converters();
    if (const auto iter = converters.find(row_view_array.begin()->type());
        iter != converters.end())
    {
        return iter->second(row_view.key(), row_view_array);
    }

    throw celeritas_error{ "Unsupported type in mongo row data." };
}
