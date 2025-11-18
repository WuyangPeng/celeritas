#include "mongo_row_data_converter.h"
#include "database/database_data_type.h"
#include "database/database_field.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <bsoncxx/types.hpp>

#include <ranges>

celeritas::basis_database celeritas::mongo_row_data_converter::get_basis_database(const database_field_container& field_name_container, const document_element_type& row_view)
{
    const std::string key{ row_view.key() };
    const auto iter = std::ranges::find_if(field_name_container, [key](const auto& value) {
        return key == value.get_field_name();
    });

    if (iter == field_name_container.cend())
    {
        return basis_database{ "nullptr" };
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

            basis_database::string_array element{};
            if (!column.empty())
            {
                split(element, column, boost::is_any_of("|"), boost::token_compress_off);
            }

            return basis_database{ iter->get_field_name(), element };
        }

        case database_data_type::int32_array_type:
        {
            const std::string column{ row_view.get_string().value };
            basis_database::string_array element{};
            if (!column.empty())
            {
                split(element, column, boost::is_any_of("|"), boost::token_compress_off);
            }

            basis_database::int32_array result{};
            for (const auto& value : element)
            {
                if (!value.empty())
                {
                    result.emplace_back(boost::lexical_cast<int32_t>(value));
                }
            }

            return basis_database{ iter->get_field_name(), result };
        }

        case database_data_type::int64_array_type:
        {
            const std::string column{ row_view.get_string().value };
            basis_database::string_array element{};
            if (!column.empty())
            {
                split(element, column, boost::is_any_of("|"), boost::token_compress_off);
            }

            basis_database::int64_array result{};
            for (const auto& value : element)
            {
                if (!value.empty())
                {
                    result.emplace_back(boost::lexical_cast<int64_t>(value));
                }
            }

            return basis_database{ iter->get_field_name(), result };
        }

        case database_data_type::double_array_type:
        {
            const std::string column{ row_view.get_string().value };
            basis_database::string_array element{};
            if (!column.empty())
            {
                split(element, column, boost::is_any_of("|"), boost::token_compress_off);
            }

            basis_database::double_array result{};
            for (const auto& value : element)
            {
                if (!value.empty())
                {
                    result.emplace_back(boost::lexical_cast<double>(value));
                }
            }

            return basis_database{ iter->get_field_name(), element };
        }

        case database_data_type::byte_array_type:
        {
            const auto binary = row_view.get_binary();
            const basis_database::byte_array result{ binary.bytes, binary.bytes + binary.size };

            return basis_database{ iter->get_field_name(), result };
        }

        default:
            return basis_database{ iter->get_field_name(), std::string{} };
    }
}
