#include "mysql_row_data_converter.tpp"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_data_type.h"
#include "database/basic/database_field.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <ranges>

celeritas::basis_database celeritas::mysql_row_data_converter::get_basis_database(const database_field& field_name, const field_view_type& row_view)
{
    switch (field_name.get_data_type())
    {
        case database_data_type::string_type:
        case database_data_type::document_type:
        {
            return basis_database{ field_name.get_field_name(), row_view.as_string() };
        }

        case database_data_type::int32_type:
        case database_data_type::int32_count_type:
        {
            return basis_database{ field_name.get_field_name(), boost::numeric_cast<int32_t>(row_view.as_int64()) };
        }

        case database_data_type::int64_type:
        case database_data_type::int64_count_type:
        {
            return basis_database{ field_name.get_field_name(), row_view.as_int64() };
        }
        case database_data_type::double_type:
        {
            return basis_database{ field_name.get_field_name(), row_view.as_double() };
        }

        case database_data_type::bool_type:
        {
            return basis_database{ field_name.get_field_name(), row_view.as_int64() != 0 };
        }

        case database_data_type::string_array_type:
        case database_data_type::document_array_type:
        {
            std::string value{ row_view.as_string() };
            if (value.starts_with("[") && value.ends_with("]"))
            {
                value = value.substr(1, value.length() - 2);
            }

            basis_database::string_array result{};
            if (!value.empty())
            {
                basis_database::string_array tokens{};
                split(tokens, value, boost::is_any_of(","), boost::token_compress_off);
                result.reserve(tokens.size());
                for (const auto& token : tokens)
                {
                    if (token.starts_with("\"") && token.ends_with("\"") && token.length() >= 2)
                    {
                        result.emplace_back(token.substr(1, token.length() - 2));
                    }
                    else
                    {
                        result.emplace_back(token);
                    }
                }
            }

            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::int32_array_type:
        {
            return to_numeric_array_basis<basis_database::int32_array>(field_name, row_view);
        }

        case database_data_type::int64_array_type:
        {
            return to_numeric_array_basis<basis_database::int64_array>(field_name, row_view);
        }

        case database_data_type::double_array_type:
        {
            return to_numeric_array_basis<basis_database::double_array>(field_name, row_view);
        }

        case database_data_type::byte_array_type:
        {
            const std::string value{ row_view.as_string() };

            const basis_database::byte_array result{ value.cbegin(), value.cend() };

            return basis_database{ field_name.get_field_name(), result };
        }

        default:
        {
            return basis_database{ field_name.get_field_name(), std::string{} };
        }
    }
}