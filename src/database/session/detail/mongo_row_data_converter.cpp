#include "mongo_row_data_converter.tpp"
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
    const auto iter = std::ranges::find_if(field_name_container, [key = row_view.key()](const auto& value) {
        return key == value.get_field_name();
    });

    if (iter == field_name_container.cend())
    {
        throw celeritas_error{ "field name is error,name ="s + row_view.key().data() };
    }

    using get_basis_from_field_function = std::function<basis_database(const document_element_type&)>;
    using container_type = std::map<database_data_type, get_basis_from_field_function>;

    static const container_type container{
        { database_data_type::string_type, [](const document_element_type& row) {
            return basis_database{ row.key(), std::string{ row.get_string().value } };
        } },
        { database_data_type::int32_type, [](const document_element_type& row) {
            return basis_database{ row.key(), row.get_int32() };
        } },
        { database_data_type::int32_count_type, [](const document_element_type& row) {
            return basis_database{ row.key(), row.get_int32() };
        } },
        { database_data_type::int64_type, [](const document_element_type& row) {
            if (row.type() == bsoncxx::type::k_int32)
            {
                return basis_database{ row.key(), row.get_int32() };
            }
            return basis_database{ row.key(), row.get_int64() };
        } },
        { database_data_type::int64_count_type, [](const document_element_type& row) {
            if (row.type() == bsoncxx::type::k_int32)
            {
                return basis_database{ row.key(), row.get_int32() };
            }
            return basis_database{ row.key(), row.get_int64() };
        } },
        { database_data_type::double_type, [](const document_element_type& row) {
            return basis_database{ row.key(), row.get_double().value };
        } },
        { database_data_type::bool_type, [](const document_element_type& row) {
            return basis_database{ row.key(), row.get_bool() };
        } },
        { database_data_type::string_array_type, [](const document_element_type& row) {
            return basis_database{ row.key(), get_numeric_array<std::string>(row.get_array().value) };
        } },
        { database_data_type::int32_array_type, [](const document_element_type& row) {
            return basis_database{ row.key(), get_numeric_array<int32_t>(row.get_array().value) };
        } },
        { database_data_type::int64_array_type, [](const document_element_type& row) {
            return basis_database{ row.key(), get_numeric_array<int64_t>(row.get_array().value) };
        } },
        { database_data_type::double_array_type, [](const document_element_type& row) {
            return basis_database{ row.key(), get_numeric_array<double>(row.get_array().value) };
        } },
        { database_data_type::byte_array_type, [](const document_element_type& row) {
            const auto binary = row.get_binary();
            const basis_database::byte_array result{ binary.bytes, binary.bytes + binary.size };
            return basis_database{ row.key(), result };
        } },
        { database_data_type::document_type, get_document_basis_database },
        { database_data_type::document_array_type, get_document_array_basis_database },
    };

    if (const auto it = container.find(iter->get_data_type());
        it != container.end())
    {
        return it->second(row_view);
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
    using get_basis_from_element_function = std::function<basis_database(const document_element_type&)>;
    using container_type = std::map<bsoncxx::type, get_basis_from_element_function>;

    static const container_type container{
        { bsoncxx::type::k_double, [](const document_element_type& row) {
            return basis_database{ row.key().data(), row.get_double().value };
        } },
        { bsoncxx::type::k_string, [](const document_element_type& row) {
            return basis_database{ row.key().data(), std::string{ row.get_string().value } };
        } },
        { bsoncxx::type::k_bool, [](const document_element_type& row) {
            return basis_database{ row.key().data(), row.get_bool().value };
        } },
        { bsoncxx::type::k_document, [](const document_element_type& row) {
            basis_database::document_type document{};
            for (const auto& element : row.get_document().value)
            {
                document.emplace_back(get_basis_database(element));
            }
            return basis_database{ row.key().data(), document };
        } },
        { bsoncxx::type::k_int32, [](const document_element_type& row) {
            return basis_database{ row.key().data(), row.get_int32().value };
        } },
        { bsoncxx::type::k_int64, [](const document_element_type& row) {
            return basis_database{ row.key().data(), row.get_int64().value };
        } },
        { bsoncxx::type::k_array, get_array_basis_database_from_view },
    };

    if (const auto iter = container.find(row_view.type());
        iter != container.end())
    {
        return iter->second(row_view);
    }

    throw celeritas_error{ "Unsupported type in mongo row data." };
}

void celeritas::mongo_row_data_converter::append_document(document_type& document, const basis_database& database)
{
    using append_function = std::function<void(document_type&, const basis_database&)>;
    using container_type = std::map<database_data_type, append_function>;

    static const container_type container{
        { database_data_type::string_type, append_basic_type<database_data_type::string_type> },
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
        { database_data_type::byte_array_type, [](document_type& document, const basis_database& basis_database) {
            const auto& byteArray = basis_database.get_value<database_data_type::byte_array_type>();
            document.append(bsoncxx::builder::basic::kvp(std::string{ basis_database.get_field_name() }, bsoncxx::types::b_binary{ bsoncxx::binary_sub_type::k_binary, static_cast<uint32_t>(byteArray.size()), byteArray.data() }));
        } },
        { database_data_type::document_type, append_document_item },
        { database_data_type::document_array_type, append_document_array_item },
    };

    if (const auto iter = container.find(database.get_data_type());
        iter != container.end())
    {
        iter->second(document, database);
    }
    else
    {
        throw celeritas_error{ "Unsupported type in mongo row data." };
    }
}

celeritas::mongo_row_data_converter::document_array celeritas::mongo_row_data_converter::get_document_array_from_view(const array_type& row_view_array)
{
    document_array database_array{};

    for (const auto& element : row_view_array)
    {
        basis_database::document_type document_type{};
        for (const auto& doc = element.get_document().value;
             const auto& value : doc)
        {
            document_type.emplace_back(get_basis_database(value));
        }
        database_array.emplace_back(document_type);
    }

    return database_array;
}

celeritas::basis_database celeritas::mongo_row_data_converter::get_document_basis_database(const document_element_type& row_view)
{
    const bsoncxx::document::value doc_value{ row_view.get_document().value };
    basis_database::document_type document{};
    for (const auto& element : doc_value)
    {
        document.emplace_back(get_basis_database(element));
    }
    return basis_database{ row_view.key().data(), document };
}

celeritas::basis_database celeritas::mongo_row_data_converter::get_document_array_basis_database(const document_element_type& row_view)
{
    const bsoncxx::document::value doc_value{ row_view.get_array().value };
    basis_database::document_array result{};
    for (const auto& element : doc_value)
    {
        basis_database::document_type document{};
        for (const auto& value : element.get_document().value)
        {
            document.emplace_back(get_basis_database(value));
        }

        result.emplace_back(document);
    }

    return basis_database{ row_view.key().data(), result };
}

celeritas::basis_database celeritas::mongo_row_data_converter::get_array_basis_database_from_view(const document_element_type& row_view)
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
