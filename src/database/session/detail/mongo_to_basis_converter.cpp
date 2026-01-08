#include "mongo_to_basis_converter.tpp"
#include "mongo_row_data_converter.tpp"
#include "database/basic/basis_database.tpp"

#include <bsoncxx/types.hpp>

const celeritas::mongo_to_basis_converter::container_type& celeritas::mongo_to_basis_converter::get_basis_from_field()
{
    static const container_type container{ { database_data_type::string_type, get_string_basis },
                                           { database_data_type::int32_type, get_int32_basis },
                                           { database_data_type::int32_count_type, get_int32_basis },
                                           { database_data_type::int64_type, get_int64_basis },
                                           { database_data_type::int64_count_type, get_int64_basis },
                                           { database_data_type::double_type, get_double_basis },
                                           { database_data_type::bool_type, get_bool_basis },
                                           { database_data_type::string_array_type, get_string_array_basis },
                                           { database_data_type::int32_array_type, get_int32_array_basis },
                                           { database_data_type::int64_array_type, get_int64_array_basis },
                                           { database_data_type::double_array_type, get_double_array_basis },
                                           { database_data_type::byte_array_type, get_byte_array_basis },
                                           { database_data_type::document_type, get_document_basis_database },
                                           { database_data_type::document_array_type, get_document_array_basis_database },
    };

    return container;
}

celeritas::basis_database celeritas::mongo_to_basis_converter::get_string_basis(const document_element_type& row_view)
{
    return basis_database{ row_view.key(), std::string{ row_view.get_string().value } };
}

celeritas::basis_database celeritas::mongo_to_basis_converter::get_int32_basis(const document_element_type& row_view)
{
    return basis_database{ row_view.key(), row_view.get_int32() };
}

celeritas::basis_database celeritas::mongo_to_basis_converter::get_int64_basis(const document_element_type& row_view)
{
    return basis_database{ row_view.key(), row_view.type() == bsoncxx::type::k_int32 ? row_view.get_int32() : row_view.get_int64() };
}

celeritas::basis_database celeritas::mongo_to_basis_converter::get_double_basis(const document_element_type& row_view)
{
    return basis_database{ row_view.key(), row_view.get_double().value };
}

celeritas::basis_database celeritas::mongo_to_basis_converter::get_bool_basis(const document_element_type& row_view)
{
    return basis_database{ row_view.key(), row_view.get_bool() };
}

celeritas::basis_database celeritas::mongo_to_basis_converter::get_string_array_basis(const document_element_type& row_view)
{
    return basis_database{ row_view.key(), get_numeric_array<std::string>(row_view.get_array().value) };
}

celeritas::basis_database celeritas::mongo_to_basis_converter::get_int32_array_basis(const document_element_type& row_view)
{
    return basis_database{ row_view.key(), get_numeric_array<int32_t>(row_view.get_array().value) };
}

celeritas::basis_database celeritas::mongo_to_basis_converter::get_int64_array_basis(const document_element_type& row_view)
{
    return basis_database{ row_view.key(), get_numeric_array<int64_t>(row_view.get_array().value) };
}

celeritas::basis_database celeritas::mongo_to_basis_converter::get_double_array_basis(const document_element_type& row_view)
{
    return basis_database{ row_view.key(), get_numeric_array<double>(row_view.get_array().value) };
}

celeritas::basis_database celeritas::mongo_to_basis_converter::get_byte_array_basis(const document_element_type& row_view)
{
    const auto binary = row_view.get_binary();
    const basis_database::byte_array result{ binary.bytes, binary.bytes + binary.size };
    return basis_database{ row_view.key(), result };
}

celeritas::basis_database celeritas::mongo_to_basis_converter::get_document_basis_database(const document_element_type& row_view)
{
    const bsoncxx::document::value doc_value{ row_view.get_document().value };
    basis_database::document_type document{};
    for (const auto& element : doc_value)
    {
        document.emplace_back(mongo_row_data_converter::get_basis_database(element));
    }
    return basis_database{ row_view.key().data(), document };
}

celeritas::basis_database celeritas::mongo_to_basis_converter::get_document_array_basis_database(const document_element_type& row_view)
{
    const bsoncxx::document::value doc_value{ row_view.get_array().value };
    basis_database::document_array result{};
    for (const auto& element : doc_value)
    {
        basis_database::document_type document{};
        for (const auto& value : element.get_document().value)
        {
            document.emplace_back(mongo_row_data_converter::get_basis_database(value));
        }

        result.emplace_back(document);
    }

    return basis_database{ row_view.key().data(), result };
}

