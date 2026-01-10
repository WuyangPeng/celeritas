#include "mongo_array_to_basis_converter.tpp"
#include "mongo_row_data_converter.h"

const celeritas::mongo_array_to_basis_converter::container_type& celeritas::mongo_array_to_basis_converter::get_converters()
{
    static const container_type container{ { bsoncxx::type::k_double, get_double_array },
                                           { bsoncxx::type::k_string, get_string_array },
                                           { bsoncxx::type::k_document, get_document_array },
                                           { bsoncxx::type::k_int32, get_int32_array },
                                           { bsoncxx::type::k_int64, get_int64_array } };

    return container;
}

celeritas::basis_database celeritas::mongo_array_to_basis_converter::get_double_array(std::string_view key, const array_type& array)
{
    return { key, get_array_from_view<double>(array) };
}

celeritas::basis_database celeritas::mongo_array_to_basis_converter::get_string_array(std::string_view key, const array_type& array)
{
    return { key, get_array_from_view<std::string>(array) };
}

celeritas::basis_database celeritas::mongo_array_to_basis_converter::get_document_array(std::string_view key, const array_type& array)
{
    return { key, get_document_array_from_view(array) };
}

celeritas::basis_database celeritas::mongo_array_to_basis_converter::get_int32_array(std::string_view key, const array_type& array)
{
    return { key, get_array_from_view<int32_t>(array) };
}

celeritas::basis_database celeritas::mongo_array_to_basis_converter::get_int64_array(std::string_view key, const array_type& array)
{
    return { key, get_array_from_view<int64_t>(array) };
}

celeritas::mongo_array_to_basis_converter::document_array celeritas::mongo_array_to_basis_converter::get_document_array_from_view(const array_type& row_view_array)
{
    document_array database_array{};

    for (const auto& element : row_view_array)
    {
        basis_database::document_type document{};
        for (const auto& doc = element.get_document().value;
             const auto& value : doc)
        {
            document.emplace_back(mongo_row_data_converter::get_basis_database(value));
        }
        database_array.emplace_back(document);
    }

    return database_array;
}
