#include "mongo_row_data_converter.tpp"
#include "mongo_to_basis_converter.tpp"
#include "mongo_element_to_basis_converter.h"
#include "common/core/celeritas_error.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/basis_database_container.h"
#include "database/basic/database_data_type.h"
#include "database/basic/database_field.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
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

celeritas::mongo_row_data_converter::document_type celeritas::mongo_row_data_converter::get_document(const basis_database_container_const_shared_ptr& container)
{
    document_type document{};

    for (const auto& value : *container)
    {
        append_document(document, value);
    }

    return document;
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

void celeritas::mongo_row_data_converter::append_document(document_type& document, const basis_database& database)
{
    using append_function = std::function<void(document_type&, const basis_database&)>;
    using container_type = std::map<database_data_type, append_function>;

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
                                           { database_data_type::document_array_type, append_document_array_item },
    };

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

void celeritas::mongo_row_data_converter::append_document_item(document_type& document, const basis_database& basis_database)
{
    bsoncxx::builder::basic::document current_document{};
    for (const auto& element : basis_database.get_value<database_data_type::document_type>())
    {
        append_document(current_document, element);
    }
    document.append(bsoncxx::builder::basic::kvp(std::string{ basis_database.get_field_name() }, current_document));
}

void celeritas::mongo_row_data_converter::append_document_array_item(document_type& document, const basis_database& basis_database)
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

void celeritas::mongo_row_data_converter::append_byte_array(document_type& document, const basis_database& basis_database)
{
    const auto& byte_array = basis_database.get_value<database_data_type::byte_array_type>();
    document.append(bsoncxx::builder::basic::kvp(
        std::string{ basis_database.get_field_name() },
        bsoncxx::types::b_binary{ bsoncxx::binary_sub_type::k_binary,
                                  static_cast<uint32_t>(byte_array.size()),
                                  byte_array.data() }));
}
