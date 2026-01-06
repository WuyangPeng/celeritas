#include "basis_database.tpp"
#include "database_data_type.h"
#include "database_data_type_traits.h"

#include <boost/numeric/conversion/cast.hpp>

celeritas::basis_database::basis_database(const std::string_view field_name)
    : class_type{ field_name, database_data_type::null_type, nullptr }
{
}

celeritas::basis_database::basis_database(const std::string_view field_name, const std::string& value)
    : class_type{ field_name, database_data_type::string_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view field_name, const int32_t value)
    : class_type{ field_name, database_data_type::int32_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view field_name, const int64_t value)
    : class_type{ field_name, database_data_type::int64_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view field_name, const uint64_t value)
    : class_type{ field_name, database_data_type::int64_type, boost::numeric_cast<int64_t>(value) }
{
}

celeritas::basis_database::basis_database(const std::string_view field_name, const document_type& value)
    : class_type{ field_name, database_data_type::document_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view field_name, const byte_array& value)
    : class_type{ field_name, database_data_type::byte_array_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view field_name, const bool value)
    : class_type{ field_name, database_data_type::bool_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view field_name, const string_array& value)
    : class_type{ field_name, database_data_type::string_array_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view field_name, const int32_array& value)
    : class_type{ field_name, database_data_type::int32_array_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view field_name, const int64_array& value)
    : class_type{ field_name, database_data_type::int64_array_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view field_name, const double_array& value)
    : class_type{ field_name, database_data_type::double_array_type, value }
{
}

celeritas::basis_database::basis_database(const std::string_view field_name, const document_array& value)
    : class_type{ field_name, database_data_type::document_array_type, value }
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
        return "`" + get_value<database_data_type::string_type>() + "`";
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
                return get_value<database_data_type::string_type>();
            }

            case database_data_type::int32_type:
            {
                return std::to_string(get_value<database_data_type::int32_type>());
            }

            case database_data_type::int32_count_type:
            {
                return std::to_string(get_value<database_data_type::int32_count_type>());
            }

            case database_data_type::int64_type:
            {
                return std::to_string(get_value<database_data_type::int64_type>());
            }

            case database_data_type::int64_count_type:
            {
                return std::to_string(get_value<database_data_type::int64_count_type>());
            }

            case database_data_type::double_type:
            {
                return std::to_string(get_value<database_data_type::double_type>());
            }

            case database_data_type::bool_type:
            {
                if (const auto result = get_value<database_data_type::bool_type>();
                    result)
                {
                    return "true";
                }
                return "false";
            }

            case database_data_type::string_array_type:
            {
                return get_array_string_value<database_data_type::string_array_type>();
            }

            case database_data_type::int32_array_type:
            {
                return get_array_string_value<database_data_type::int32_array_type>();
            }

            case database_data_type::int64_array_type:
            {
                return get_array_string_value<database_data_type::int64_array_type>();
            }

            case database_data_type::double_array_type:
            {
                return get_array_string_value<database_data_type::double_array_type>();
            }

            case database_data_type::byte_array_type:
            {
                const auto& byteArray = get_value<database_data_type::byte_array_type>();
                return std::string{ byteArray.begin(), byteArray.end() };
            }

            case database_data_type::document_type:
            {
                return get_document_string();
            }

            case database_data_type::document_array_type:
            {
                return get_document_array_string();
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
        return "\"" + get_value<database_data_type::string_type>() + "\"";
    }

    return get_string();
}

celeritas::basis_database::basis_database(const std::string_view field_name, const database_data_type dataType, std::any value)
    : field_name_{ field_name }, data_type_{ dataType }, value_{ std::move(value) }
{
}

std::string celeritas::basis_database::get_document_string() const
{
    return "";
}

std::string celeritas::basis_database::get_document_array_string() const
{
    return "";
}

bool celeritas::operator==(const basis_database& lhs, const basis_database& rhs)
{
    if (lhs.get_data_type() != rhs.get_data_type())
    {
        return false;
    }

    if (lhs.get_field_name() != rhs.get_field_name())
    {
        return false;
    }
    switch (lhs.get_data_type())
    {
        case database_data_type::null_type:
        {
            return true;
        }
        case database_data_type::string_type:
        {
            return lhs.get_value<database_data_type::string_type>() == rhs.get_value<database_data_type::string_type>();
        }
        case database_data_type::string_array_type:
        {
            return lhs.get_value<database_data_type::string_array_type>() == rhs.get_value<database_data_type::string_array_type>();
        }
        case database_data_type::int32_type:
        {
            return lhs.get_value<database_data_type::int32_type>() == rhs.get_value<database_data_type::int32_type>();
        }
        case database_data_type::int32_count_type:
        {
            return lhs.get_value<database_data_type::int32_count_type>() == rhs.get_value<database_data_type::int32_count_type>();
        }
        case database_data_type::int32_array_type:
        {
            return lhs.get_value<database_data_type::int32_array_type>() == rhs.get_value<database_data_type::int32_array_type>();
        }
        case database_data_type::int64_type:
        {
            return lhs.get_value<database_data_type::int64_type>() == rhs.get_value<database_data_type::int64_type>();
        }
        case database_data_type::int64_count_type:
        {
            return lhs.get_value<database_data_type::int64_count_type>() == rhs.get_value<database_data_type::int64_count_type>();
        }
        case database_data_type::int64_array_type:
        {
            return lhs.get_value<database_data_type::int64_array_type>() == rhs.get_value<database_data_type::int64_array_type>();
        }
        case database_data_type::double_type:
        {
            return lhs.get_value<database_data_type::double_type>() == rhs.get_value<database_data_type::double_type>();
        }
        case database_data_type::double_array_type:
        {
            return lhs.get_value<database_data_type::double_array_type>() == rhs.get_value<database_data_type::double_array_type>();
        }
        case database_data_type::bool_type:
        {
            return lhs.get_value<database_data_type::bool_type>() == rhs.get_value<database_data_type::bool_type>();
        }
        case database_data_type::byte_array_type:
        {
            return lhs.get_value<database_data_type::byte_array_type>() == rhs.get_value<database_data_type::byte_array_type>();
        }
        case database_data_type::document_type:
        {
            return lhs.get_value<database_data_type::document_type>() == rhs.get_value<database_data_type::document_type>();
        }
        case database_data_type::document_array_type:
        {
            return lhs.get_value<database_data_type::document_array_type>() == rhs.get_value<database_data_type::document_array_type>();
        }
    }

    return false;
}

bool celeritas::operator!=(const basis_database& lhs, const basis_database& rhs)
{
    return !(lhs == rhs);
}
