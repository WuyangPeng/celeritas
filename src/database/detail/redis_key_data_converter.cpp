#include "redis_key_data_converter.h"
#include "common/celeritas_error.h"
#include "database/basis_database.tpp"
#include "database/database_data_type.h"
#include "database/database_field.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <ranges>

std::string celeritas::redis_key_data_converter::generate_key(const basis_database_manager_const_shared_ptr& database)
{
    std::string result{};
    result += database->get_database_name();
    result += ":";

    auto index = 1;
    for (const auto keys = database->get_key();
         const auto& key : keys)
    {
        result += key.get_quotation_mark_string();

        if (index != keys.get_size())
        {
            result += "_";
        }

        ++index;
    }

    return result;
}

celeritas::basis_database celeritas::redis_key_data_converter::get_basis_database(const database_field& field_name, const std::string& value)
{
    switch (field_name.get_data_type())
    {
        case database_data_type::string_type:
        {
            return basis_database{ field_name.get_field_name(), value };
        }

        case database_data_type::int32_type:
        case database_data_type::int32_count_type:
        {
            return basis_database{ field_name.get_field_name(), boost::lexical_cast<int32_t>(value) };
        }

        case database_data_type::int64_type:
        case database_data_type::int64_count_type:
        {
            return basis_database{ field_name.get_field_name(), boost::lexical_cast<int64_t>(value) };
        }

        case database_data_type::double_type:
        {
            return basis_database{ field_name.get_field_name(), boost::lexical_cast<double>(value) };
        }

        case database_data_type::bool_type:
        {
            return basis_database{ field_name.get_field_name(), value == "true" };
        }

        case database_data_type::string_array_type:
        {
            basis_database::string_array result{};
            split(result, value, boost::is_any_of("|"), boost::token_compress_off);

            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::int32_array_type:
        {
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
            auto split_view = value | std::views::split('|');

            auto int_view = split_view | std::views::transform([](const auto& subrange) {
                const std::string result{ subrange.begin(), subrange.end() };
                return boost::lexical_cast<double>(result);
            });
            const basis_database::double_array result{ int_view.begin(), int_view.end() };

            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::byte_array_type:
        {
            const basis_database::byte_array result{ value.cbegin(), value.cend() };

            return basis_database{ field_name.get_field_name(), result };
        }

        default:
        {
            return basis_database{ field_name.get_field_name(), std::string{} };
        }
    }
}

celeritas::basis_database_container celeritas::redis_key_data_converter::get_key(const std::string& key, const basis_database_manager_const_shared_ptr& database)
{
    const auto extracted_key_values = get_key_value(key);

    const auto key_type = database->get_key();
    if (extracted_key_values.size() != key_type.get_size())
    {
        throw celeritas_error("key size is error.");
    }

    basis_database_container::object_container objects{};
    auto index = 0;
    for (const auto& value : key_type)
    {
        objects.emplace_back(value.get_field_name(), extracted_key_values.at(index));
        ++index;
    }

    return basis_database_container{ objects };
}

celeritas::redis_key_data_converter::array_type celeritas::redis_key_data_converter::get_key_value(const std::string& key)
{
    array_type parts{};
    boost::split(parts, key, boost::is_any_of(":"), boost::token_compress_off);

    if (parts.size() < 2)
    {
        throw celeritas_error("redis key size is error.");
    }

    const auto& key_values = parts.back();

    array_type result{};
    boost::split(result, key_values, boost::is_any_of("_"), boost::token_compress_off);

    return result;
}