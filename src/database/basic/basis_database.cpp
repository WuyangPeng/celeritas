#include "basis_database.tpp"

#include <boost/numeric/conversion/cast.hpp>
#include <functional>
#include <map>
#include <sstream>

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
    if (!value_.has_value())
    {
        return "";
    }

    using converter_function = std::function<std::string(const basis_database*)>;
    static const std::map<database_data_type, converter_function> converters{
        { database_data_type::string_type, [](const basis_database* db) {
            return db->get_value<database_data_type::string_type>();
        } },
        { database_data_type::int32_type, [](const basis_database* db) {
            return std::to_string(db->get_value<database_data_type::int32_type>());
        } },
        { database_data_type::int32_count_type, [](const basis_database* db) {
            return std::to_string(db->get_value<database_data_type::int32_count_type>());
        } },
        { database_data_type::int64_type, [](const basis_database* db) {
            return std::to_string(db->get_value<database_data_type::int64_type>());
        } },
        { database_data_type::int64_count_type, [](const basis_database* db) {
            return std::to_string(db->get_value<database_data_type::int64_count_type>());
        } },
        { database_data_type::double_type, [](const basis_database* db) {
            return std::to_string(db->get_value<database_data_type::double_type>());
        } },
        { database_data_type::bool_type, [](const basis_database* db) {
            return db->get_value<database_data_type::bool_type>() ? "true" : "false";
        } },
        { database_data_type::string_array_type, [](const basis_database* db) {
            return db->get_array_string_value<database_data_type::string_array_type>();
        } },
        { database_data_type::int32_array_type, [](const basis_database* db) {
            return db->get_array_string_value<database_data_type::int32_array_type>();
        } },
        { database_data_type::int64_array_type, [](const basis_database* db) {
            return db->get_array_string_value<database_data_type::int64_array_type>();
        } },
        { database_data_type::double_array_type, [](const basis_database* db) {
            return db->get_array_string_value<database_data_type::double_array_type>();
        } },
        { database_data_type::byte_array_type, [](const basis_database* db) {
            const auto& byteArray = db->get_value<database_data_type::byte_array_type>();
            return std::string{ byteArray.begin(), byteArray.end() };
        } },
        { database_data_type::document_type, [](const basis_database* db) {
            return db->get_document_string();
        } },
        { database_data_type::document_array_type, [](const basis_database* db) {
            return db->get_document_array_string();
        } }
    };

    if (const auto iter = converters.find(data_type_);
        iter != converters.end())
    {
        return iter->second(this);
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
    const auto& document = get_value<database_data_type::document_type>();
    if (document.empty())
    {
        return "{}";
    }

    std::stringstream ss{};
    ss << "{";
    for (auto iter = document.cbegin(); iter != document.cend(); ++iter)
    {
        ss << "\"" << iter->get_field_name() << "\":" << iter->get_quotation_mark_string();
        if (std::next(iter) != document.cend())
        {
            ss << ",";
        }
    }
    ss << "}";

    return ss.str();
}

std::string celeritas::basis_database::get_document_array_string() const
{
    const auto& doc_array = get_value<database_data_type::document_array_type>();
    if (doc_array.empty())
    {
        return "[]";
    }

    std::stringstream ss{};
    ss << "[";
    for (auto iter = doc_array.cbegin(); iter != doc_array.cend(); ++iter)
    {
        basis_database doc{ "", database_data_type::document_type, *iter };
        ss << doc.get_string();
        if (std::next(iter) != doc_array.cend())
        {
            ss << ",";
        }
    }
    ss << "]";
    return ss.str();
}

bool celeritas::operator==(const basis_database& lhs, const basis_database& rhs)
{
    if (lhs.get_data_type() != rhs.get_data_type() || lhs.get_field_name() != rhs.get_field_name())
    {
        return false;
    }

    using comparator_function = std::function<bool(const basis_database&, const basis_database&)>;
    static const std::map<database_data_type, comparator_function> comparators{
        { database_data_type::null_type, [](const basis_database& l, const basis_database& r) {
            return true;
        } },
        { database_data_type::string_type, [](const basis_database& l, const basis_database& r) {
            return l.get_value<database_data_type::string_type>() == r.get_value<database_data_type::string_type>();
        } },
        { database_data_type::string_array_type, [](const basis_database& l, const basis_database& r) {
            return l.get_value<database_data_type::string_array_type>() == r.get_value<database_data_type::string_array_type>();
        } },
        { database_data_type::int32_type, [](const basis_database& l, const basis_database& r) {
            return l.get_value<database_data_type::int32_type>() == r.get_value<database_data_type::int32_type>();
        } },
        { database_data_type::int32_count_type, [](const basis_database& l, const basis_database& r) {
            return l.get_value<database_data_type::int32_count_type>() == r.get_value<database_data_type::int32_count_type>();
        } },
        { database_data_type::int32_array_type, [](const basis_database& l, const basis_database& r) {
            return l.get_value<database_data_type::int32_array_type>() == r.get_value<database_data_type::int32_array_type>();
        } },
        { database_data_type::int64_type, [](const basis_database& l, const basis_database& r) {
            return l.get_value<database_data_type::int64_type>() == r.get_value<database_data_type::int64_type>();
        } },
        { database_data_type::int64_count_type, [](const basis_database& l, const basis_database& r) {
            return l.get_value<database_data_type::int64_count_type>() == r.get_value<database_data_type::int64_count_type>();
        } },
        { database_data_type::int64_array_type, [](const basis_database& l, const basis_database& r) {
            return l.get_value<database_data_type::int64_array_type>() == r.get_value<database_data_type::int64_array_type>();
        } },
        { database_data_type::double_type, [](const basis_database& l, const basis_database& r) {
            return l.get_value<database_data_type::double_type>() == r.get_value<database_data_type::double_type>();
        } },
        { database_data_type::double_array_type, [](const basis_database& l, const basis_database& r) {
            return l.get_value<database_data_type::double_array_type>() == r.get_value<database_data_type::double_array_type>();
        } },
        { database_data_type::bool_type, [](const basis_database& l, const basis_database& r) {
            return l.get_value<database_data_type::bool_type>() == r.get_value<database_data_type::bool_type>();
        } },
        { database_data_type::byte_array_type, [](const basis_database& l, const basis_database& r) {
            return l.get_value<database_data_type::byte_array_type>() == r.get_value<database_data_type::byte_array_type>();
        } },
        { database_data_type::document_type, [](const basis_database& l, const basis_database& r) {
            return l.get_value<database_data_type::document_type>() == r.get_value<database_data_type::document_type>();
        } },
        { database_data_type::document_array_type, [](const basis_database& l, const basis_database& r) {
            return l.get_value<database_data_type::document_array_type>() == r.get_value<database_data_type::document_array_type>();
        } }
    };

    if (const auto iter = comparators.find(lhs.get_data_type());
        iter != comparators.end())
    {
        return iter->second(lhs, rhs);
    }

    return false;
}

bool celeritas::operator!=(const basis_database& lhs, const basis_database& rhs)
{
    return !(lhs == rhs);
}
