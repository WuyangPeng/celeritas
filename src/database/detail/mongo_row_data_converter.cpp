#include "mongo_row_data_converter.tpp"
#include "common/celeritas_error.h"
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
            return basis_database{ iter->get_field_name(), database_data_type::document_type, bsoncxx::to_json(doc_value) };
        }

        case database_data_type::document_array_type:
        {
            const bsoncxx::document::value doc_value{ row_view.get_document().value };
            basis_database::string_array result{};
            for (const auto& element : doc_value)
            {
                result.emplace_back(bsoncxx::to_json(element.get_document().value));
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
                const auto document_type = value.get_value<database_data_type::document_type>();
                auto doc_value = bsoncxx::from_json(document_type);

                document.append(bsoncxx::builder::basic::kvp(fieldName, doc_value));
                break;
            }
            case database_data_type::document_array_type:
            {
                const auto document_type = value.get_value<database_data_type::document_array_type>();
                bsoncxx::builder::basic::array basic{};
                for (const auto& element : document_type)
                {
                    auto doc_value = bsoncxx::from_json(element);
                    basic.append(doc_value);
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
