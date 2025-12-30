#include "mongo_row_data_converter.tpp"
#include "common/core_utilities/celeritas_error.h"
#include "database/basis_database.tpp"
#include "database/basis_database_container.h"
#include "database/database_data_type.h"
#include "database/database_field.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/builder/basic/array.hpp>

using namespace std::literals;

celeritas::basis_database celeritas::mongo_row_data_converter::get_basis_database(const database_field_container& field_name_container, const document_element_type& row_view)
{
    const auto iter = std::ranges::find_if(field_name_container, [key = row_view.key()](const auto& value) {
        return key == value.get_field_name();
    });

    if (iter == field_name_container.cend())
    {
        throw celeritas_error{ "field name is error,name ="s + row_view.key().data() };
    }

    switch (iter->get_data_type())
    {
        case database_data_type::string_type:
        {
            const std::string result{ row_view.get_string().value };
            return basis_database{ iter->get_field_name(), result };
        }

        case database_data_type::int32_type:
        case database_data_type::int32_count_type:
        {
            return basis_database{ iter->get_field_name(), row_view.get_int32() };
        }

        case database_data_type::int64_type:
        case database_data_type::int64_count_type:
        {
            if (row_view.type() == bsoncxx::type::k_int32)
            {
                return basis_database{ iter->get_field_name(), row_view.get_int32() };
            }
            return basis_database{ iter->get_field_name(), row_view.get_int64() };
        }

        case database_data_type::double_type:
        {
            return basis_database{ iter->get_field_name(), row_view.get_double().value };
        }

        case database_data_type::bool_type:
        {
            return basis_database{ iter->get_field_name(), row_view.get_bool() };
        }

        case database_data_type::string_array_type:
        {
            return basis_database{ iter->get_field_name(), get_numeric_array<std::string>(row_view.get_array().value) };
        }

        case database_data_type::int32_array_type:
        {
            return basis_database{ iter->get_field_name(), get_numeric_array<int32_t>(row_view.get_array().value) };
        }

        case database_data_type::int64_array_type:
        {
            return basis_database{ iter->get_field_name(), get_numeric_array<int64_t>(row_view.get_array().value) };
        }

        case database_data_type::double_array_type:
        {
            return basis_database{ iter->get_field_name(), get_numeric_array<double>(row_view.get_array().value) };
        }

        case database_data_type::byte_array_type:
        {
            const auto binary = row_view.get_binary();
            const basis_database::byte_array result{ binary.bytes, binary.bytes + binary.size };

            return basis_database{ iter->get_field_name(), result };
        }

        case database_data_type::document_type:
        {
            const bsoncxx::document::value doc_value{ row_view.get_document().value };
            basis_database::document_type document{};
            for (const auto& element : doc_value)
            {
                document.emplace_back(get_basis_database(element));
            }
            return basis_database{ iter->get_field_name(), database_data_type::document_type, document };
        }

        case database_data_type::document_array_type:
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

            return basis_database{ iter->get_field_name(), database_data_type::document_array_type, result };
        }
        default:
        {
            return basis_database{ iter->get_field_name(), std::string{} };
        }
    }
}

celeritas::mongo_row_data_converter::document_type celeritas::mongo_row_data_converter::get_document(const basis_database_container_const_shared_ptr& container)
{
    document_type document{};

    for (const auto& value : *container)
    {
        std::string fieldName{ value.get_field_name() };
        switch (value.get_data_type())
        {
            case database_data_type::string_type:
            {
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_value<database_data_type::string_type>()));
                break;
            }

            case database_data_type::int32_type:
            case database_data_type::int32_count_type:
            {
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_value<database_data_type::int32_type>()));
                break;
            }

            case database_data_type::int64_type:
            case database_data_type::int64_count_type:
            {
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_value<database_data_type::int64_type>()));
                break;
            }

            case database_data_type::double_type:
            {
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_value<database_data_type::double_type>()));
                break;
            }

            case database_data_type::bool_type:
            {
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_value<database_data_type::bool_type>()));
                break;
            }

            case database_data_type::string_array_type:
            {
                bsoncxx::builder::basic::array basic{};
                for (const auto& element : value.get_value<database_data_type::string_array_type>())
                {
                    basic.append(element);
                }
                document.append(bsoncxx::builder::basic::kvp(fieldName, basic));
                break;
            }

            case database_data_type::int32_array_type:
            {
                bsoncxx::builder::basic::array basic{};
                for (const auto& element : value.get_value<database_data_type::int32_array_type>())
                {
                    basic.append(element);
                }
                document.append(bsoncxx::builder::basic::kvp(fieldName, basic));
                break;
            }

            case database_data_type::int64_array_type:
            {
                bsoncxx::builder::basic::array basic{};
                for (const auto& element : value.get_value<database_data_type::int64_array_type>())
                {
                    basic.append(element);
                }
                document.append(bsoncxx::builder::basic::kvp(fieldName, basic));
                break;
            }

            case database_data_type::double_array_type:
            {
                bsoncxx::builder::basic::array basic{};
                for (const auto& element : value.get_value<database_data_type::double_array_type>())
                {
                    basic.append(element);
                }
                document.append(bsoncxx::builder::basic::kvp(fieldName, basic));
                break;
            }

            case database_data_type::byte_array_type:
            {
                const auto& byteArray = value.get_value<database_data_type::byte_array_type>();
                document.append(bsoncxx::builder::basic::kvp(fieldName, bsoncxx::types::b_binary{ bsoncxx::binary_sub_type::k_binary, static_cast<uint32_t>(byteArray.size()), byteArray.data() }));
                break;
            }

            case database_data_type::document_type:
            {
                bsoncxx::builder::basic::document current_document{};
                for (const auto document_type = value.get_value<database_data_type::document_type>();
                     const auto& element : document_type)
                {
                    append_document(current_document, element);
                }

                document.append(bsoncxx::builder::basic::kvp(fieldName, current_document));
                break;
            }
            case database_data_type::document_array_type:
            {
                const auto document_type = value.get_value<database_data_type::document_array_type>();
                bsoncxx::builder::basic::array basic{};
                for (const auto& element : document_type)
                {
                    bsoncxx::builder::basic::document current_document{};
                    for (const auto& database_data : element)
                    {
                        append_document(current_document, database_data);
                    }
                    basic.append(current_document);
                }

                document.append(bsoncxx::builder::basic::kvp(fieldName, basic));
                break;
            }
            default:
                break;
        }
    }

    return document;
}

celeritas::basis_database celeritas::mongo_row_data_converter::get_basis_database(const document_element_type& row_view)
{
    switch (row_view.type())
    {
        case bsoncxx::type::k_double:
        {
            return basis_database{ row_view.key().data(), row_view.get_double().value };
        }
        case bsoncxx::type::k_string:
        {
            return basis_database{ row_view.key().data(), std::string{ row_view.get_string().value } };
        }
        case bsoncxx::type::k_bool:
        {
            return basis_database{ row_view.key().data(), row_view.get_bool().value };
        }
        case bsoncxx::type::k_document:
        {
            basis_database::document_type document{};
            for (const auto& element : row_view.get_document().value)
            {
                document.emplace_back(get_basis_database(element));
            }
            return basis_database{ row_view.key().data(), document };
        }
        case bsoncxx::type::k_int32:
        {
            return basis_database{ row_view.key().data(), row_view.get_int32().value };
        }
        case bsoncxx::type::k_int64:
        {
            return basis_database{ row_view.key().data(), row_view.get_int64().value };
        }
        case bsoncxx::type::k_array:
        {
            const auto row_view_array = row_view.get_array().value;
            if (row_view_array.empty())
            {
                return basis_database{ row_view.key().data(), basis_database::int32_array{} };
            }
            const auto sub_type = row_view_array.begin()->type();
            switch (sub_type)
            {
                case bsoncxx::type::k_double:
                {
                    basis_database::double_array database_array{};
                    for (const auto& element : row_view_array)
                    {
                        database_array.emplace_back(element.get_double().value);
                    }
                    return basis_database{ row_view.key().data(), database_array };
                }
                case bsoncxx::type::k_string:
                {
                    basis_database::string_array database_array{};
                    for (const auto& element : row_view_array)
                    {
                        database_array.emplace_back(element.get_string().value);
                    }
                    return basis_database{ row_view.key().data(), database_array };
                }
                case bsoncxx::type::k_document:
                {
                    basis_database::document_array database_array{};
                    for (const auto& element : row_view_array)
                    {
                        basis_database::document_type document_type{};
                        const auto& doc = element.get_document().value;
                        for (const auto& value : doc)
                        {
                            document_type.emplace_back(get_basis_database(value));
                        }
                        database_array.emplace_back(document_type);
                    }
                    return basis_database{ row_view.key().data(), database_array };
                }
                case bsoncxx::type::k_int32:
                {
                    basis_database::int32_array database_array{};
                    for (const auto& element : row_view_array)
                    {
                        database_array.emplace_back(element.get_int32().value);
                    }
                    return basis_database{ row_view.key().data(), database_array };
                }
                case bsoncxx::type::k_int64:
                {
                    basis_database::int64_array database_array{};
                    for (const auto& element : row_view_array)
                    {
                        database_array.emplace_back(element.get_int64().value);
                    }
                    return basis_database{ row_view.key().data(), database_array };
                }
                default:
                {
                    throw celeritas_error{ "Unsupported type in mongo row data." };
                }
            }
        }
        default:
        {
            throw celeritas_error{ "Unsupported type in mongo row data." };
        }
    }
}

void celeritas::mongo_row_data_converter::append_document(document_type& document, const basis_database& basis_database)
{
    switch (basis_database.get_data_type())
    {
        case database_data_type::null_type:
        {
            break;
        }

        case database_data_type::string_type:
        {
            const auto result = basis_database.get_value<database_data_type::string_type>();
            document.append(bsoncxx::builder::basic::kvp(std::string{ basis_database.get_field_name() }, result));
            break;
        }
        case database_data_type::string_array_type:
        {
            bsoncxx::builder::basic::array basic{};
            for (const auto& element : basis_database.get_value<database_data_type::string_array_type>())
            {
                basic.append(element);
            }
            document.append(bsoncxx::builder::basic::kvp(basis_database.get_field_name(), basic));
            break;
        }
        case database_data_type::int32_type:
        {
            document.append(bsoncxx::builder::basic::kvp(basis_database.get_field_name(), basis_database.get_value<database_data_type::int32_type>()));
            break;
        }
        case database_data_type::int32_count_type:
        {
            document.append(bsoncxx::builder::basic::kvp(basis_database.get_field_name(), basis_database.get_value<database_data_type::int32_count_type>()));
            break;
        }
        case database_data_type::int32_array_type:
        {
            bsoncxx::builder::basic::array basic{};
            for (const auto& element : basis_database.get_value<database_data_type::int32_array_type>())
            {
                basic.append(element);
            }
            document.append(bsoncxx::builder::basic::kvp(basis_database.get_field_name(), basic));
            break;
        }
        case database_data_type::int64_type:
        {
            document.append(bsoncxx::builder::basic::kvp(basis_database.get_field_name(), basis_database.get_value<database_data_type::int64_type>()));
            break;
        }
        case database_data_type::int64_count_type:
        {
            document.append(bsoncxx::builder::basic::kvp(basis_database.get_field_name(), basis_database.get_value<database_data_type::int64_count_type>()));
            break;
        }
        case database_data_type::int64_array_type:
        {
            bsoncxx::builder::basic::array basic{};
            for (const auto& element : basis_database.get_value<database_data_type::int64_array_type>())
            {
                basic.append(element);
            }
            document.append(bsoncxx::builder::basic::kvp(basis_database.get_field_name(), basic));
            break;
        }
        case database_data_type::double_type:
        {
            document.append(bsoncxx::builder::basic::kvp(basis_database.get_field_name(), basis_database.get_value<database_data_type::double_type>()));
            break;
        }
        case database_data_type::double_array_type:
        {
            bsoncxx::builder::basic::array basic{};
            for (const auto& element : basis_database.get_value<database_data_type::double_array_type>())
            {
                basic.append(element);
            }
            document.append(bsoncxx::builder::basic::kvp(basis_database.get_field_name(), basic));
            break;
        }
        case database_data_type::bool_type:
        {
            document.append(bsoncxx::builder::basic::kvp(basis_database.get_field_name(), basis_database.get_value<database_data_type::bool_type>()));
            break;
        }
        case database_data_type::byte_array_type:
        {
            const auto& byteArray = basis_database.get_value<database_data_type::byte_array_type>();
            document.append(bsoncxx::builder::basic::kvp(basis_database.get_field_name(), bsoncxx::types::b_binary{ bsoncxx::binary_sub_type::k_binary, static_cast<uint32_t>(byteArray.size()), byteArray.data() }));
            break;
        }
        case database_data_type::document_type:
        {
            bsoncxx::builder::basic::document current_document{};
            for (const auto& element : basis_database.get_value<database_data_type::document_type>())
            {
                append_document(current_document, element);
            }
            document.append(bsoncxx::builder::basic::kvp(basis_database.get_field_name(), current_document));

            break;
        }

        case database_data_type::document_array_type:
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
            document.append(bsoncxx::builder::basic::kvp(basis_database.get_field_name(), current_document));
            break;
        }

        default:
        {
            throw celeritas_error{ "Unsupported type in mongo row data." };
        }
    }
}
