#include "mongo_row_data_converter.h"
#include "common/celeritas_error.h"
#include "database/basis_database.tpp"
#include "database/basis_database_container.h"
#include "database/database_data_type.h"
#include "database/database_field.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <bsoncxx/types.hpp>

#include <ranges>

using namespace std::literals;

celeritas::basis_database celeritas::mongo_row_data_converter::get_basis_database(const database_field_container& field_name_container, const document_element_type& row_view)
{
    const auto iter = std::ranges::find_if(field_name_container, [key = row_view.key()](const auto& value) {
        return key == value.get_field_name();
    });

    if (iter == field_name_container.cend())
    {
        throw celeritas_error("field name is error,name ="s + row_view.key().data());
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
            return basis_database{ iter->get_field_name(), row_view.get_int32() };

        case database_data_type::int64_type:
        case database_data_type::int64_count_type:
            return basis_database{ iter->get_field_name(), row_view.get_int64() };

        case database_data_type::double_type:
            return basis_database{ iter->get_field_name(), row_view.get_double().value };

        case database_data_type::bool_type:
            return basis_database{ iter->get_field_name(), row_view.get_bool() };

        case database_data_type::string_array_type:
        {
            const std::string column{ row_view.get_string().value };

            basis_database::string_array result{};
            if (!column.empty())
            {
                split(result, column, boost::is_any_of("|"), boost::token_compress_off);
            }

            return basis_database{ iter->get_field_name(), result };
        }

        case database_data_type::int32_array_type:
        {
            const std::string value{ row_view.get_string().value };
            auto split_view = value | std::views::split('|');

            auto int_view = split_view | std::views::transform([](const auto& subrange) {
                const std::string result{ subrange.begin(), subrange.end() };
                return boost::lexical_cast<int32_t>(result);
            });
            const basis_database::int32_array result{ int_view.begin(), int_view.end() };

            return basis_database{ iter->get_field_name(), result };
        }

        case database_data_type::int64_array_type:
        {
            const std::string value{ row_view.get_string().value };
            auto split_view = value | std::views::split('|');

            auto int_view = split_view | std::views::transform([](const auto& subrange) {
                const std::string result{ subrange.begin(), subrange.end() };
                return boost::lexical_cast<int64_t>(result);
            });
            const basis_database::int64_array result{ int_view.begin(), int_view.end() };

            return basis_database{ iter->get_field_name(), result };
        }

        case database_data_type::double_array_type:
        {
            const std::string value{ row_view.get_string().value };
            auto split_view = value | std::views::split('|');

            auto int_view = split_view | std::views::transform([](const auto& subrange) {
                const std::string result{ subrange.begin(), subrange.end() };
                return boost::lexical_cast<double>(result);
            });
            const basis_database::double_array result{ int_view.begin(), int_view.end() };

            return basis_database{ iter->get_field_name(), result };
        }

        case database_data_type::byte_array_type:
        {
            const auto binary = row_view.get_binary();
            const basis_database::byte_array result{ binary.bytes, binary.bytes + binary.size };

            return basis_database{ iter->get_field_name(), result };
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
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_array_string_value<database_data_type::string_array_type>()));
                break;
            }

            case database_data_type::int32_array_type:
            {
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_array_string_value<database_data_type::int32_array_type>()));
                break;
            }

            case database_data_type::int64_array_type:
            {
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_array_string_value<database_data_type::int64_array_type>()));
                break;
            }

            case database_data_type::double_array_type:
            {
                document.append(bsoncxx::builder::basic::kvp(fieldName, value.get_array_string_value<database_data_type::double_array_type>()));
                break;
            }

            case database_data_type::byte_array_type:
            {
                const auto& byteArray = value.get_value<database_data_type::byte_array_type>();
                document.append(bsoncxx::builder::basic::kvp(fieldName, bsoncxx::types::b_binary{ bsoncxx::binary_sub_type::k_binary, static_cast<uint32_t>(byteArray.size()), byteArray.data() }));
                break;
            }

            default:
                break;
        }
    }

    return document;
}
