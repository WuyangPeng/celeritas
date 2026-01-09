#include "basis_to_mongo_appender.tpp"
#include "common/core/celeritas_error.h"

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/types.hpp>

const celeritas::basis_to_mongo_appender::container_type& celeritas::basis_to_mongo_appender::get_appender()
{
    static const container_type container{ { database_data_type::string_type, append_basic_type<database_data_type::string_type> },
                                           { database_data_type::string_array_type, append_array_document<database_data_type::string_array_type> },
                                           { database_data_type::int32_type, append_basic_type<database_data_type::int32_type> },
                                           { database_data_type::int32_count_type, append_basic_type<database_data_type::int32_count_type> },
                                           { database_data_type::int32_array_type, append_array_document<database_data_type::int32_array_type> },
                                           { database_data_type::int64_type, append_basic_type<database_data_type::int64_type> },
                                           { database_data_type::int64_count_type, append_basic_type<database_data_type::int64_count_type> },
                                           { database_data_type::int64_array_type, append_array_document<database_data_type::int64_array_type> },
                                           { database_data_type::double_type, append_basic_type<database_data_type::double_type> },
                                           { database_data_type::double_array_type, append_array_document<database_data_type::double_array_type> },
                                           { database_data_type::bool_type, append_basic_type<database_data_type::bool_type> },
                                           { database_data_type::byte_array_type, append_byte_array },
                                           { database_data_type::document_type, append_document_item },
                                           { database_data_type::document_array_type, append_document_array_item } };

    return container;
}

void celeritas::basis_to_mongo_appender::append_document(document_type& document, const basis_database& database)
{
    const auto& container = get_appender();

    if (const auto iter = container.find(database.get_data_type());
        iter != container.cend())
    {
        iter->second(document, database);
    }
    else
    {
        throw celeritas_error{ "Unsupported type in mongo row data." };
    }
}

void celeritas::basis_to_mongo_appender::append_document_item(document_type& document, const basis_database& basis_database)
{
    bsoncxx::builder::basic::document current_document{};
    for (const auto& element : basis_database.get_value<database_data_type::document_type>())
    {
        append_document(current_document, element);
    }
    document.append(bsoncxx::builder::basic::kvp(std::string{ basis_database.get_field_name() }, current_document));
}

void celeritas::basis_to_mongo_appender::append_document_array_item(document_type& document, const basis_database& basis_database)
{
    bsoncxx::builder::basic::array current_document{};
    for (const auto& element : basis_database.get_value<database_data_type::document_array_type>())
    {
        bsoncxx::builder::basic::document current{};
        for (const auto& value : element)
        {
            append_document(current, value);
        }

        current_document.append(current);
    }
    document.append(bsoncxx::builder::basic::kvp(std::string{ basis_database.get_field_name() }, current_document));
}

void celeritas::basis_to_mongo_appender::append_byte_array(document_type& document, const basis_database& basis_database)
{
    const auto& byte_array = basis_database.get_value<database_data_type::byte_array_type>();
    document.append(bsoncxx::builder::basic::kvp(std::string{ basis_database.get_field_name() },
                                                 bsoncxx::types::b_binary{ bsoncxx::binary_sub_type::k_binary,
                                                                           static_cast<uint32_t>(byte_array.size()),
                                                                           byte_array.data() }));
}

