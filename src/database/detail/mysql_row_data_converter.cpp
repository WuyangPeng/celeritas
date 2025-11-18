#include "mysql_row_data_converter.h"
#include "database/database_data_type.h"
#include "database/database_field.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <ranges>

celeritas::basis_database celeritas::mysql_row_data_converter::get_basis_database(const database_field& field_name, const field_view_type& row_view)
{
    switch (field_name.get_data_type())
    {
        case database_data_type::string_type:
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
        {
            basis_database::string_array result{};
            split(result, row_view.as_string(), boost::is_any_of("|"), boost::token_compress_off);

            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::int32_array_type:
        {
            const std::string value{ row_view.as_string() };
            auto split_view = value | std::views::split('|');

            auto int_view = split_view | std::views::transform([](const auto& subrange) {
                const std::string result{ subrange.begin(), subrange.end() };
                return boost::lexical_cast<int32_t>(result);
            });
            const basis_database::int32_array result{ int_view.begin(), int_view.end() };

            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::int64_array_type:
        {
            const std::string value{ row_view.as_string() };
            auto split_view = value | std::views::split('|');

            auto int_view = split_view | std::views::transform([](const auto& subrange) {
                const std::string result{ subrange.begin(), subrange.end() };
                return boost::lexical_cast<int64_t>(result);
            });
            const basis_database::int64_array result{ int_view.begin(), int_view.end() };

            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::double_array_type:
        {
            const std::string value{ row_view.as_string() };
            auto split_view = value | std::views::split('|');

            auto int_view = split_view | std::views::transform([](const auto& subrange) {
                const std::string result{ subrange.begin(), subrange.end() };
                return boost::lexical_cast<double>(result);
            });
            const basis_database::double_array result{ int_view.begin(), int_view.end() };

            return basis_database{ field_name.get_field_name(), result };
        }

        default:
        {
            return basis_database{ field_name.get_field_name(), std::string{} };
        }
    }
}