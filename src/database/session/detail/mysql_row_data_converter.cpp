#include "mysql_row_data_converter.tpp"
#include "common/logging/logger.h"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_data_type.h"
#include "database/basic/database_field.h"
#include "json_value_to_basis_converter.h"

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

        case database_data_type::document_type:
        {
            std::string value{ row_view.as_string() };
            if (value.empty())
            {
                return basis_database{ field_name.get_field_name(), basis_database::document_type{} };
            }

            try
            {
                auto jv = boost::json::parse(value);
                if (jv.is_object())
                {
                    basis_database::document_type doc{};
                    const auto& obj = jv.get_object();
                    doc.reserve(obj.size());
                    for (const auto& kv : obj)
                    {
                        doc.emplace_back(json_value_to_basis_converter::convert(kv.key(), kv.value()));
                    }
                    return basis_database{ field_name.get_field_name(), doc };
                }
            }
            catch (const std::exception& exception)
            {
                LOG_CHANNEL(database_channel, error) << "database data json error:" << exception.what();
            }

            return basis_database{ field_name.get_field_name(), value };
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
            std::string value{ row_view.as_string() };
            if (value.empty())
            {
                return basis_database{ field_name.get_field_name(), basis_database::string_array{} };
            }

            const auto json = boost::json::parse(value);
            const auto result = boost::json::value_to<basis_database::string_array>(json);

            return basis_database{ field_name.get_field_name(), result };
        }

        case database_data_type::document_array_type:
        {
            std::string value{ row_view.as_string() };
            if (value.empty())
            {
                return basis_database{ field_name.get_field_name(), basis_database::document_array{} };
            }

            try
            {
                auto jv = boost::json::parse(value);
                if (jv.is_array())
                {
                    basis_database::document_array result{};
                    const auto& arr = jv.get_array();
                    result.reserve(arr.size());
                    for (const auto& el : arr)
                    {
                        if (el.is_object())
                        {
                            basis_database::document_type doc;
                            const auto& obj = el.get_object();
                            doc.reserve(obj.size());
                            for (const auto& kv : obj)
                            {
                                doc.emplace_back(json_value_to_basis_converter::convert(kv.key(), kv.value()));
                            }
                            result.emplace_back(std::move(doc));
                        }
                    }
                    return basis_database{ field_name.get_field_name(), result };
                }
            }
            catch (const std::exception& exception)
            {
                LOG_CHANNEL(database_channel, error) << "database data json error:" << exception.what();
            }
            return basis_database{ field_name.get_field_name(), basis_database::document_array{} };
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
