#include "basis_database.h"
#include "database_data_type.h"
#include "database_data_type_traits.h"

#include <boost/numeric/conversion/cast.hpp>

celeritas::basis_database::basis_database(const std::string_view fieldName)
    : class_type{ fieldName, database_data_type::null_type, nullptr }
{
}

celeritas::basis_database::basis_database(const std::string_view fieldName, const std::string& value)
    : class_type{ fieldName, database_data_type::string_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view fieldName, const int32_t value)
    : class_type{ fieldName, database_data_type::int32_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view fieldName, const int64_t value)
    : class_type{ fieldName, database_data_type::int64_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view fieldName, const uint64_t value)
    : class_type{ fieldName, database_data_type::int64_type, boost::numeric_cast<int64_t>(value) }
{
}

celeritas::basis_database::basis_database(const std::string_view fieldName, const byte_array& value)
    : class_type{ fieldName, database_data_type::byte_array_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view fieldName, const bool value)
    : class_type{ fieldName, database_data_type::bool_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view fieldName, const string_array& value)
    : class_type{ fieldName, database_data_type::string_array_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view fieldName, const int32_array& value)
    : class_type{ fieldName, database_data_type::int32_array_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view fieldName, const int64_array& value)
    : class_type{ fieldName, database_data_type::int64_array_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view fieldName, const double_array& value)
    : class_type{ fieldName, database_data_type::double_array_type, value }
{
}

std::string_view celeritas::basis_database::get_field_name() const noexcept
{
    return field_name_;
}

celeritas::database_data_type celeritas::basis_database::get_data_type() const noexcept
{
    return data_type_;
}

std::any celeritas::basis_database::get_any_value() const
{
    return value_;
}

std::string celeritas::basis_database::get_sql_field_string() const
{
    if (data_type_ == database_data_type::string_type)
    {
        return "`" + std::any_cast<database_data_Type_traits<database_data_type::string_type>::Type>(value_) + "`";
    }

    return get_string();
}

std::string celeritas::basis_database::get_string() const
{
    if (value_.has_value())
    {
        switch (data_type_)
        {
            case database_data_type::string_type:
            {
                return std::any_cast<database_data_Type_traits<database_data_type::string_type>::Type>(value_);
            }

            case database_data_type::int32_type:
            {
                return std::to_string(std::any_cast<database_data_Type_traits<database_data_type::int32_type>::Type>(value_));
            }

            case database_data_type::int32_count_type:
            {
                return std::to_string(std::any_cast<database_data_Type_traits<database_data_type::int32_count_type>::Type>(value_));
            }

            case database_data_type::int64_type:
            {
                return std::to_string(std::any_cast<database_data_Type_traits<database_data_type::int64_type>::Type>(value_));
            }

            case database_data_type::int64_count_type:
            {
                return std::to_string(std::any_cast<database_data_Type_traits<database_data_type::int64_count_type>::Type>(value_));
            }

            case database_data_type::double_type:
            {
                return std::to_string(std::any_cast<database_data_Type_traits<database_data_type::double_type>::Type>(value_));
            }

            case database_data_type::bool_type:
            {
                if (const auto result = std::any_cast<database_data_Type_traits<database_data_type::bool_type>::Type>(value_);
                    result)
                {
                    return "true";
                }
 return "false";
            }

            case database_data_type::string_array_type:
            {
                const auto anyValue = std::any_cast<database_data_Type_traits<database_data_type::string_array_type>::Type>(value_);

                std::string result{};

                auto index = 0u;
                for (const auto& element : anyValue)
                {
                    result += element;

                    ++index;

                    if (index != anyValue.size())
                    {
                        result += "|";
                    }
                }

                return result;
            }

            case database_data_type::int32_array_type:
            {
                const auto anyValue = std::any_cast<database_data_Type_traits<database_data_type::int32_array_type>::Type>(value_);

                std::string result{};

                auto index = 0u;
                for (const auto& element : anyValue)
                {
                    result += std::to_string(element);

                    ++index;

                    if (index != anyValue.size())
                    {
                        result += "|";
                    }
                }

                return result;
            }

            case database_data_type::int64_array_type:
            {
                const auto anyValue = std::any_cast<database_data_Type_traits<database_data_type::int64_array_type>::Type>(value_);

                std::string result{};

                auto index = 0u;
                for (const auto& element : anyValue)
                {
                    result += std::to_string(element);

                    ++index;

                    if (index != anyValue.size())
                    {
                        result += "|";
                    }
                }

                return result;
            }

            case database_data_type::double_array_type:
            {
                const auto anyValue = std::any_cast<database_data_Type_traits<database_data_type::double_array_type>::Type>(value_);

                std::string result{};

                auto index = 0u;
                for (const auto& element : anyValue)
                {
                    result += std::to_string(element);

                    ++index;

                    if (index != anyValue.size())
                    {
                        result += "|";
                    }
                }

                return result;
            }

            default:
            {
                return "";
            }
        }
    }

    return "";
}

std::string celeritas::basis_database::get_quotation_mark_string() const
{
    if (data_type_ == database_data_type::string_type)
    {
        return "\"" + std::any_cast<database_data_Type_traits<database_data_type::string_type>::Type>(value_) + "\"";
    }

    return get_string();
}

celeritas::basis_database::basis_database(const std::string_view fieldName, const database_data_type dataType, std::any value)
    : field_name_{ fieldName }, data_type_{ dataType }, value_{ std::move(value) }
{
}