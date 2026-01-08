#include "mongo_element_to_basis_converter.tpp"
#include "mongo_row_data_converter.tpp"
#include "common/core/celeritas_error.h"

const celeritas::mongo_element_to_basis_converter::container_type& celeritas::mongo_element_to_basis_converter::get_element_converters()
{
    static const container_type container{ { bsoncxx::type::k_double, get_double_from_element },
                                           { bsoncxx::type::k_string, get_string_from_element },
                                           { bsoncxx::type::k_bool, get_bool_from_element },
                                           { bsoncxx::type::k_document, get_document_from_element },
                                           { bsoncxx::type::k_int32, get_int32_from_element },
                                           { bsoncxx::type::k_int64, get_int64_from_element },
                                           { bsoncxx::type::k_array, get_array_basis_database_from_view },
    };

    return container;
}

celeritas::basis_database celeritas::mongo_element_to_basis_converter::get_double_from_element(const document_element_type& row)
{
    return { row.key().data(), row.get_double().value };
}

celeritas::basis_database celeritas::mongo_element_to_basis_converter::get_string_from_element(const document_element_type& row)
{
    return { row.key().data(), std::string{ row.get_string().value } };
}

celeritas::basis_database celeritas::mongo_element_to_basis_converter::get_bool_from_element(const document_element_type& row)
{
    return { row.key().data(), row.get_bool().value };
}

celeritas::basis_database celeritas::mongo_element_to_basis_converter::get_document_from_element(const document_element_type& row)
{
    basis_database::document_type document{};
    for (const auto& element : row.get_document().value)
    {
        document.emplace_back(mongo_row_data_converter::get_basis_database(element));
    }
    return { row.key().data(), document };
}

celeritas::basis_database celeritas::mongo_element_to_basis_converter::get_int32_from_element(const document_element_type& row)
{
    return { row.key().data(), row.get_int32().value };
}

celeritas::basis_database celeritas::mongo_element_to_basis_converter::get_int64_from_element(const document_element_type& row)
{
    return { row.key().data(), row.get_int64().value };
}

celeritas::basis_database celeritas::mongo_element_to_basis_converter::get_array_basis_database_from_view(const document_element_type& row_view)
{
    const auto row_view_array = row_view.get_array().value;
    if (row_view_array.empty())
    {
        return { row_view.key().data(), basis_database::int32_array{} };
    }

    switch (row_view_array.begin()->type())
    {
        case bsoncxx::type::k_double:
        {
            return { row_view.key().data(), get_array_from_view<double>(row_view_array) };
        }
        case bsoncxx::type::k_string:
        {
            return { row_view.key().data(), get_array_from_view<std::string>(row_view_array) };
        }
        case bsoncxx::type::k_document:
        {
            return { row_view.key().data(), get_document_array_from_view(row_view_array) };
        }
        case bsoncxx::type::k_int32:
        {
            return { row_view.key().data(), get_array_from_view<int32_t>(row_view_array) };
        }
        case bsoncxx::type::k_int64:
        {
            return { row_view.key().data(), get_array_from_view<int64_t>(row_view_array) };
        }
        default:
        {
            throw celeritas_error{ "Unsupported type in mongo row data." };
        }
    }
}

celeritas::mongo_element_to_basis_converter::document_array celeritas::mongo_element_to_basis_converter::get_document_array_from_view(const array_type& row_view_array)
{
    document_array database_array{};

    for (const auto& element : row_view_array)
    {
        basis_database::document_type document_type{};
        for (const auto& doc = element.get_document().value;
             const auto& value : doc)
        {
            document_type.emplace_back(mongo_row_data_converter::get_basis_database(value));
        }
        database_array.emplace_back(document_type);
    }

    return database_array;
}
