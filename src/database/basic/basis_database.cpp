#include "basis_database.tpp"

#include <boost/numeric/conversion/cast.hpp>

#include <sstream>

celeritas::basis_database::basis_database(const std::string_view field_name)
    : class_type{ field_name, database_data_type::null_type, std::monostate{} }
{
}

celeritas::basis_database::basis_database(const std::string_view field_name, const char* value)
    : class_type{ field_name, database_data_type::string_type, value }
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

std::string celeritas::basis_database::get_field_name() const noexcept
{
    return field_name_;
}

celeritas::database_data_type celeritas::basis_database::get_data_type() const noexcept
{
    return data_type_;
}

const celeritas::basis_database::value_variant& celeritas::basis_database::get_variant_value() const
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
    return std::visit(overloaded{
                          [](std::monostate) {
                              return std::string{};
                          },
                          [](const std::string& value) {
                              return value;
                          },
                          [](const int32_t value) {
                              return std::to_string(value);
                          },
                          [](const int64_t value) {
                              return std::to_string(value);
                          },
                          [](const double value) {
                              return std::to_string(value);
                          },
                          [](const bool value) {
                              return std::string{ value ? "true" : "false" };
                          },
                          [](const byte_array& value) {
                              return std::string{ value.begin(), value.end() };
                          },
                          [this](const string_array& value) {
                              return get_array_string_value<database_data_type::string_array_type>();
                          },
                          [this](const int32_array& value) {
                              return get_array_string_value<database_data_type::int32_array_type>();
                          },
                          [this](const int64_array& value) {
                              return get_array_string_value<database_data_type::int64_array_type>();
                          },
                          [this](const double_array& value) {
                              return get_array_string_value<database_data_type::double_array_type>();
                          },
                          [this](const document_type& value) {
                              return get_document_string();
                          },
                          [this](const document_array& value) {
                              return get_document_array_string();
                          }
                      }, value_);
}

std::string celeritas::basis_database::get_quotation_mark_string() const
{
    if (data_type_ == database_data_type::string_type)
    {
        return "\"" + get_value<database_data_type::string_type>() + "\"";
    }

    return get_string();
}

std::string celeritas::basis_database::get_sql_value_string() const
{
    if (data_type_ == database_data_type::string_type)
    {
        return "\"" + get_value<database_data_type::string_type>() + "\"";
    }

    if (data_type_ == database_data_type::string_array_type ||
        data_type_ == database_data_type::int32_array_type ||
        data_type_ == database_data_type::int64_array_type ||
        data_type_ == database_data_type::double_array_type ||
        data_type_ == database_data_type::document_type ||
        data_type_ == database_data_type::document_array_type)
    {
        return "'" + get_value<database_data_type::string_type>() + "'";
    }

    return get_string();
}

celeritas::basis_database::basis_database(const std::string_view field_name, const database_data_type dataType, value_variant value)
    : field_name_{ field_name }, data_type_{ dataType }, value_{ std::move(value) }
{
}

std::string celeritas::basis_database::get_document_string() const
{
    const auto& document = get_value<database_data_type::document_type>();
    if (document.empty())
    {
        return "{}";
    }

    std::ostringstream os{};
    os << "{";
    for (auto iter = document.cbegin(); iter != document.cend(); ++iter)
    {
        append_value(os, *iter, std::next(iter) == document.cend());
    }
    os << "}";

    return os.str();
}

std::string celeritas::basis_database::get_document_array_string() const
{
    const auto& doc_array = get_value<database_data_type::document_array_type>();
    if (doc_array.empty())
    {
        return "[]";
    }

    std::ostringstream os{};
    os << "[";
    for (auto iter = doc_array.cbegin(); iter != doc_array.cend(); ++iter)
    {
        append_value(os, *iter, std::next(iter) == doc_array.cend());
    }
    os << "]";
    return os.str();
}

void celeritas::basis_database::append_value(std::ostringstream& os, const basis_database& value, const bool is_last)
{
    os << "\"" << value.get_field_name() << "\":" << value.get_quotation_mark_string();
    if (!is_last)
    {
        os << ",";
    }
}

void celeritas::basis_database::append_value(std::ostringstream& os, const document_type& value, const bool is_last)
{
    const basis_database doc{ "", database_data_type::document_type, value };
    os << doc.get_string();
    if (!is_last)
    {
        os << ",";
    }
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

    return lhs.get_variant_value() == rhs.get_variant_value();
}

bool celeritas::operator!=(const basis_database& lhs, const basis_database& rhs)
{
    return !(lhs == rhs);
}
