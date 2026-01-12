#include "json_value_to_basis_converter.h"
#include "redis_to_basis_converter.tpp"
#include "database/basic/basis_database.tpp"
#include "database/basic/database_data_type.h"

#include <boost/json.hpp>
#include <boost/lexical_cast.hpp>

celeritas::basis_database celeritas::redis_to_basis_converter::get_basis_database(const database_field& field_name, const std::string& value)
{
    const auto& converters = get_converters();
    if (const auto iter = converters.find(field_name.get_data_type());
        iter != converters.cend())
    {
        return iter->second(field_name, value);
    }

    return get_default_basis(field_name, value);
}

const celeritas::redis_to_basis_converter::container_type& celeritas::redis_to_basis_converter::get_converters()
{
    static const container_type container{ { database_data_type::string_type, get_string_basis },
                                           { database_data_type::document_type, get_document_basis },
                                           { database_data_type::int32_type, get_int32_basis },
                                           { database_data_type::int32_count_type, get_int32_basis },
                                           { database_data_type::int64_type, get_int64_basis },
                                           { database_data_type::int64_count_type, get_int64_basis },
                                           { database_data_type::double_type, get_double_basis },
                                           { database_data_type::bool_type, get_bool_basis },
                                           { database_data_type::string_array_type, get_string_array_basis },
                                           { database_data_type::document_array_type, get_document_array_basis },
                                           { database_data_type::int32_array_type, get_int32_array_basis },
                                           { database_data_type::int64_array_type, get_int64_array_basis },
                                           { database_data_type::double_array_type, get_double_array_basis },
                                           { database_data_type::byte_array_type, get_byte_array_basis } };

    return container;
}

celeritas::basis_database celeritas::redis_to_basis_converter::get_string_basis(const database_field& field_name, const std::string& value)
{
    return basis_database{ field_name.get_field_name(), value };
}

celeritas::basis_database celeritas::redis_to_basis_converter::get_document_basis(const database_field& field_name, const std::string& value)
{
    if (value.empty())
    {
        return basis_database{ field_name.get_field_name(), basis_database::document_type{} };
    }

    auto json_value = boost::json::parse(value);

    basis_database::document_type document{};
    const auto& object = json_value.get_object();
    document.reserve(object.size());
    for (const auto& element : object)
    {
        document.emplace_back(json_value_to_basis_converter::convert(element.key(), element.value()));
    }
    return basis_database{ field_name.get_field_name(), document };
}

celeritas::basis_database celeritas::redis_to_basis_converter::get_int32_basis(const database_field& field_name, const std::string& value)
{
    return basis_database{ field_name.get_field_name(), boost::lexical_cast<int32_t>(value) };
}

celeritas::basis_database celeritas::redis_to_basis_converter::get_int64_basis(const database_field& field_name, const std::string& value)
{
    return basis_database{ field_name.get_field_name(), boost::lexical_cast<int64_t>(value) };
}

celeritas::basis_database celeritas::redis_to_basis_converter::get_double_basis(const database_field& field_name, const std::string& value)
{
    return basis_database{ field_name.get_field_name(), boost::lexical_cast<double>(value) };
}

celeritas::basis_database celeritas::redis_to_basis_converter::get_bool_basis(const database_field& field_name, const std::string& value)
{
    return basis_database{ field_name.get_field_name(), value == "true" };
}

celeritas::basis_database celeritas::redis_to_basis_converter::get_string_array_basis(const database_field& field_name, const std::string& value)
{
    if (value.empty())
    {
        return basis_database{ field_name.get_field_name(), basis_database::string_array{} };
    }

    const auto json = boost::json::parse(value);
    const auto result = boost::json::value_to<basis_database::string_array>(json);

    return basis_database{ field_name.get_field_name(), result };
}

celeritas::basis_database celeritas::redis_to_basis_converter::get_document_array_basis(const database_field& field_name, const std::string& value)
{
    if (value.empty())
    {
        return basis_database{ field_name.get_field_name(), basis_database::document_array{} };
    }

    auto json_value = boost::json::parse(value);

    basis_database::document_array result{};

    const auto& array = json_value.get_array();
    result.reserve(array.size());
    for (const auto& element : array)
    {
        basis_database::document_type document{};
        const auto& object = element.get_object();
        document.reserve(object.size());
        for (const auto& key_value : object)
        {
            document.emplace_back(json_value_to_basis_converter::convert(key_value.key(), key_value.value()));
        }
        result.emplace_back(std::move(document));
    }

    return basis_database{ field_name.get_field_name(), result };
}

celeritas::basis_database celeritas::redis_to_basis_converter::get_int32_array_basis(const database_field& field_name, const std::string& value)
{
    return to_numeric_array_basis<basis_database::int32_array>(field_name, value);
}

celeritas::basis_database celeritas::redis_to_basis_converter::get_int64_array_basis(const database_field& field_name, const std::string& value)
{
    return to_numeric_array_basis<basis_database::int64_array>(field_name, value);
}

celeritas::basis_database celeritas::redis_to_basis_converter::get_double_array_basis(const database_field& field_name, const std::string& value)
{
    return to_numeric_array_basis<basis_database::double_array>(field_name, value);
}

celeritas::basis_database celeritas::redis_to_basis_converter::get_byte_array_basis(const database_field& field_name, const std::string& value)
{
    const basis_database::byte_array result{ value.cbegin(), value.cend() };

    return basis_database{ field_name.get_field_name(), result };
}

celeritas::basis_database celeritas::redis_to_basis_converter::get_default_basis(const database_field& field_name, const std::string& value)
{
    return basis_database{ field_name.get_field_name(), std::string{} };
}
